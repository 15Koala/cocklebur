/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * the implementation of Cocklebur
 */
#include "Cocklebur.h"
#include <sys/stat.h>
#include <iostream>
#include "utils.h"
#include "cocklebur_constance.h"
#include "CockLeaderElection.h"
#include "CockleburServer.h"
#include "LeaderElectionServer.h"
#include "Follower.h"
#include "Leader.h"

void Cocklebur::getVoter( Voter & _voter ) {
    pthread_rwlock_rdlock( &m_host_stat );
    
    _voter.xid = d_host_stat.xid;
    _voter.rec_host = d_host_stat.rec_host;
    _voter.my_host_name = d_host_stat.my_host_name;
    _voter.logical_clock = d_host_stat.logical_clock;
    _voter.known_hosts = d_host_stat.known_hosts;
    _voter.cur_node_mode = getCurNodeMode();
    
    pthread_rwlock_unlock( &m_host_stat );
}
void Cocklebur::setCurNodeMode( CockleburMode cur_node_mode ) {
    pthread_rwlock_wrlock( &m_cur_node_mode );
    d_cur_node_mode = cur_node_mode;
    pthread_rwlock_unlock( &m_cur_node_mode );
}

void Cocklebur::updateHostStat( const Voter * _voter, bool use_lock ) {

    if( use_lock && ack_get_lock() ) return ;

    pthread_rwlock_wrlock( &m_host_stat );
    if( _voter == NULL ) { 
	return;
    } 

    if( _voter->cur_node_mode == LOOKING ) {
	if( d_host_stat.xid < _voter->xid ) {
	    // change my stat
	    d_host_stat.xid = _voter->xid;
	    d_host_stat.rec_host = _voter->rec_host;
	}else if( d_host_stat.xid == _voter->xid ){
	    // choose a bigger hostid
	    if( d_host_stat.my_host_name < _voter->my_host_name ) {
		d_host_stat.xid = _voter->xid;
		d_host_stat.rec_host = _voter->rec_host;
	    }
	}
	// add to heard_from
	d_host_stat.heard_from.insert( _voter->my_host_name );
	// add to known_hosts
	d_host_stat.known_hosts.insert( _voter->known_hosts.begin(), _voter->known_hosts.end() );
    } 
    pthread_rwlock_unlock( &m_host_stat );
}
//cocklebur constructor
Cocklebur::Cocklebur(map<string,string> & map_conf){

    d_conf = map_conf;

    if( map_conf.count("cocklebur.data.dir") > 0 ) d_conf_data_dir = map_conf["cocklebur.data.dir"];

    if( d_conf_data_dir == "" ) d_conf_data_dir = "cocklebur_data";

    if( map_conf.count("cocklebur.host.list") > 0 ) {
	string host_list = map_conf["cocklebur.host.list"];
	vector< string > vec_host_list;
	str_split(vec_host_list, host_list,',',true);
	vector< string >::iterator it = vec_host_list.begin();
	for( ; it != vec_host_list.end() ; it++ ) {
	    vector< string > vec_port;
	    str_split( vec_port, *it, ':' );
	    d_conf_map_host_port[vec_port[0]] = atoi( vec_port[1].c_str() );
	}
    }

    if( map_conf.count("cocklebur.server.address") >0 ) {
	vector< string > tmp;
	str_split( tmp, map_conf["cocklebur.server.address"], ':' );
	d_conf_my_ip = tmp[0];
	d_conf_my_port = atoi( tmp[1].c_str() );
    }

    if( access( d_conf_data_dir.c_str(), 0 ) != 0 ) {
	// we got nothing from data_dir
	mkdir(d_conf_data_dir.c_str(), 0775);
	initial_in_memory();
    } else {
	// we got the snapshots from data_dir
	initial_in_memory();
	//initial_in_file();
    }

    // initial locks
    pthread_rwlock_init( &m_host_stat, NULL );
    pthread_rwlock_init( &m_cur_node_mode, NULL );
    pthread_rwlock_init( &m_ack_lock, NULL );
}

Cocklebur::~Cocklebur() {
    
    pthread_join( m_id_cock_server, NULL );
    pthread_join( m_id_cock_election_server, NULL );
    pthread_join( m_id_leader_server, NULL );

    pthread_rwlock_destroy(&m_host_stat);
    pthread_rwlock_destroy(&m_cur_node_mode);
    pthread_rwlock_destroy( &m_ack_lock );

}

void Cocklebur::rollLogs( const string & dir_name ) {
    if( access( d_conf_data_dir.c_str(), 0 ) != 0 ) {
	// we got nothing from data_dir
	mkdir(d_conf_data_dir.c_str(), 0775);
    } else {
    
    }
}

void Cocklebur::initial_in_memory() {  
    // d_host_stat initialization.
    d_host_stat.xid = 0;
    map<string,int>::iterator map_it = d_conf_map_host_port.begin();
    for( ; map_it != d_conf_map_host_port.end(); map_it++ ) {
	d_host_stat.alive_hosts.insert( map_it->first );
    }

    d_host_stat.my_host_name = d_conf_my_ip;
    d_host_stat.rec_host = d_conf_my_ip;
    d_host_stat.heard_from.insert( d_conf_my_ip );
    d_host_stat.known_hosts.insert( d_conf_my_ip );

    // host mode initialization.
    // mode = looking
    d_cur_node_mode = LOOKING;
}

void Cocklebur::initial_in_file(){}

void Cocklebur::resetHostStat() {
    pthread_rwlock_wrlock( &m_host_stat );
    d_host_stat.known_hosts.clear();
    d_host_stat.known_hosts.insert( d_conf_my_ip );
    d_host_stat.heard_from.clear();
    d_host_stat.heard_from.insert( d_conf_my_ip );
    d_host_stat.rec_host = d_conf_my_ip;
    pthread_rwlock_unlock( &m_host_stat );
}

void Cocklebur::startCockServer( Leader * leader ) {

    lArg l_arg = new LeaderArg;
    l_arg->s_port = getConfMyPort() + 2;
    l_arg->s_leader = leader;

    // start leader server 
    int ret = pthread_create( &m_id_leader_server, NULL, startCockleburServer, reinterpret_cast< void * > (l_arg) );

    if( ret != 0 ) {
	return ;
    }
}

void Cocklebur::startCockElectionServer() {

    pArg p_arg = new ThreadServerArg;
    p_arg->s_port = d_conf_my_port + 1;
    p_arg->s_cocklebur = this;

    int ret = pthread_create( &m_id_cock_election_server, NULL, startLeaderElectionServer, reinterpret_cast<void *>(p_arg) );
    
    if( ret !=0 ){
	return ;
    }
}

void Cocklebur::run() {
    // cocklebur leader election server, for exchanging voters.
    startCockElectionServer();
    //main process 
    process(); 
}

void Cocklebur::process() {

    CockLeaderElection cockLeaderElection(this);
    Leader leader(this);
    startCockServer( &leader );

    while(true) {

	switch( d_cur_node_mode ) { 

	case LOOKING:
	    cout<<"_DEBUG: Enter looking process."<<endl;
	    cockLeaderElection.lookForLeader(); 
	    break;
	case FOLLWING:
	    cout<<"_DEBUG: Enter following process."<<endl;
	    // apply lease from leader
	    {
		Follower follower(this);
		follower.following();
	    }
	    break;
	case LEADERING:
	    cout<<"_DEBUG: Enter leadering process."<<endl;
	    // recevie heartbeat from followers and observers
	    {
		leader.reset();
		leader.leadering();
	    }
	    break;
	default:
	    cout<<"_DEBUG: No match mode for "<<d_cur_node_mode<<endl;
	    break;
	}
    }
}
void Cocklebur::printCockleburInfo() {

    cout<<"-----------Cocklebur Current Stat----------"<<endl;
    cout<<"|=>Configuration:"<<endl;
    cout<<"| ip:port => "<<d_conf_my_ip<<":"<<d_conf_my_port<<endl;
    cout<<"| data dir => "<<d_conf_data_dir<<endl;
    cout<<"| hosts => ";
    map<string, int>::iterator it = d_conf_map_host_port.begin();
    for( ; it != d_conf_map_host_port.end(); it++ ) {
	cout<<it->first<<":"<<it->second<<" ";
    }
    cout<<endl;
    cout<<"|=>Status:"<<endl;
    cout<<"| xid => "<<d_host_stat.xid<<endl;
    cout<<"| rec_host => "<<d_host_stat.rec_host<<endl;
    cout<<"| my_host_name => "<<d_host_stat.my_host_name<<endl;
    cout<<"-------------------------------------------"<<endl;

}
