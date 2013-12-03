#include "CockLeaderElection.h"

#include <iostream>
#include "cocklebur_constance.h"
#include "LeaderElectionClient.h"
#include "CockleburClient.h"
#include "utils.h"

using namespace std;

CockLeaderElection::CockLeaderElection(Cocklebur * c){
    d_cocklebur = c;
    d_logical_clock = 0;
    c->setLogicalClock( d_logical_clock ); // always sync

    map< string, string > map_conf = c->getConf();
    if( map_conf.count("cocklebur.host.list") > 0 ) {
	string host_list = map_conf["cocklebur.host.list"];
	vector< string > vec_host_list;
	str_split( vec_host_list, host_list, ',', true );

	int i;
	for( i = 0; i < vec_host_list.size() ; i++ ) {
	    vector< string > vec_port;
	    str_split( vec_port, vec_host_list[i], ':');
	    string my_host_name;
	    c->getMyHostName(my_host_name);
	    if( vec_port[0] != my_host_name ) {
		d_task.push_back( vec_port[0] );
	    }
	}
    }

    d_cock_client = new CockleburClient( d_cocklebur->getConfHostList() );
    // initial locks
    pthread_rwlock_init( &m_logical_clock, NULL );
    pthread_rwlock_init( &m_voter, NULL );
}
CockLeaderElection::~CockLeaderElection() {

    delete d_cock_client;

    pthread_rwlock_unlock( &m_logical_clock );
    pthread_rwlock_unlock( &m_voter );
}

long CockLeaderElection::getLogicalClock() {
    pthread_rwlock_rdlock( &m_logical_clock );
    long x = d_logical_clock;
    pthread_rwlock_unlock( &m_logical_clock );
    return x;
}
void CockLeaderElection::logicalClockTick() {
    pthread_rwlock_wrlock( &m_logical_clock );
    d_logical_clock ++;
    d_cocklebur->setLogicalClock( d_logical_clock );
    pthread_rwlock_unlock( &m_logical_clock );
}
void CockLeaderElection::logicalClockTick( long x ) {
    pthread_rwlock_wrlock( &m_logical_clock );
    d_logical_clock = x;
    d_cocklebur->setLogicalClock( d_logical_clock );
    pthread_rwlock_unlock( &m_logical_clock );
}
void CockLeaderElection::getVoter(Voter & _voter){

    pthread_rwlock_wrlock( &m_voter );
    d_cocklebur->getRecHostName(d_voter.rec_host);
    d_voter.my_host_name = d_cocklebur->getConfMyIp();
    d_voter.xid = d_cocklebur->getXid();
    d_cocklebur->getKnownHosts(d_voter.known_hosts);
    d_voter.logical_clock = d_logical_clock;
    d_voter.cur_node_mode = d_cocklebur->getCurNodeMode();
    _voter = d_voter;
    pthread_rwlock_unlock( &m_voter );
}

void CockLeaderElection::lookForLeader() {

    map<string, int> conf_host_list = d_cocklebur->getConfHostList();
    int num_hosts = conf_host_list.size();
    int num_major = num_hosts/2 + 1;

    d_cocklebur->ack_unlock();
    d_cocklebur->resetHostStat();

    LeaderElectionClient ele_client( d_cocklebur->getConfHostList() );// connection
    
    logicalClockTick();// logical_clock ++;

    set< string > t_locked_hosts;// locked hosts who ack me
    int t_cur_task = 0;// current task's subscript

    while( d_cocklebur->getCurNodeMode() == LOOKING && d_task.size() > 0 ) {
	//sleep(1);

	if( t_cur_task >= d_task.size() ) t_cur_task = 0;

	if( d_cocklebur->ack_get_lock() ) {
	    cout<<"_DEBUG: Locked myself, waiting for leader."<<endl;
	    int time_out_cnt = 0;//TODO conf
	    while( d_cocklebur->getCurNodeMode() == LOOKING && time_out_cnt <=10 ) {
		sleep(1);
		++ time_out_cnt;
	    }

	    if( d_cocklebur->getCurNodeMode() == LOOKING ) {
		// implys that ack_lock times out
		d_cocklebur->resetHostStat();// clear host stat
		d_cocklebur->ack_unlock();// unlock myself continue looking for
	    }
	}

	getVoter(d_voter);// update current voter and get voter

	Voter t_voter;
	Voter * _voter = &t_voter;
	string target_host = d_task[ t_cur_task++ ]; // get target host's voter
	if( t_locked_hosts.count( target_host ) == 0 ) {

	    ele_client.exchange( _voter, d_voter, target_host ); // exchange vote

	    if( _voter->my_host_name == "" ) cout<<"_DEBUG: Failed to exchange vote with "<<target_host<<"."<<endl;
	    // dead or locked
	    if( _voter->my_host_name == "" ) continue;
	    // too bad, my vote is expired. so break and have another lookForLeader
	    if( _voter->logical_clock > getLogicalClock() ) {
		logicalClockTick( _voter->logical_clock - 1 );
		break;
	    }

	    d_cocklebur->updateHostStat( _voter, true );
	    getVoter(d_voter);// update current voter and get voter

	    if( _voter->cur_node_mode > 1 ) {
		HBMSG message, _message;
		message.cur_node_mode = LOOKING;
		message.my_host_name = d_voter.my_host_name;
		message.xid = d_voter.xid;
		if( d_cock_client->ping2( _message, message, _voter->rec_host ) ) {
		    cout<<"_DEBUG: Check "<<_message.my_host_name<<" is a leader or not, it's mode is "<<_message.cur_node_mode<<endl;
		    if( _message.cur_node_mode != LEADERING ) continue;
		} else continue;
		d_cocklebur->setCurNodeMode( FOLLWING );
		d_cocklebur->setRecHostName( _voter->rec_host );
		d_cocklebur->ack_unlock();
		break;
	    }
	}

	cout<<"_DEBUG: Current known_hosts size "<<d_voter.known_hosts.size()<<" and major num "<<num_major<<endl;
	if( d_voter.known_hosts.size() >= num_major ) {
	    cout<<"_DEBUG: Majority hosts found, decide who is leader."<<endl;
	    // i can decide i am leader or not. 
	    if( d_voter.rec_host == d_voter.my_host_name ) {
		int ack_cnt = 0;
		// apply others to be the leader.
		cout<<"_DEBUG: My xid is largest, asking other hosts."<<endl;
		set< string >::iterator it = d_voter.known_hosts.begin();
		for( ; it != d_voter.known_hosts.end(); it ++ ) {
		    if( *it == d_voter.my_host_name ) continue;
		    if( t_locked_hosts.count( *it ) > 0 ) {
			ack_cnt ++;
		    }
		    else if( ele_client.isLargest( d_voter, *it ) ) {
			t_locked_hosts.insert( *it );
			ack_cnt ++;
		    }
		}
		cout<<"_DEBUG: Got "<<ack_cnt+1<<" ack, Major is "<<num_major<<endl;
		
		if( ack_cnt >= num_major-1 ) {
		    string cur_rec_host;
		    d_cocklebur->getRecHostName( cur_rec_host );
		    if( d_cocklebur->getConfMyIp() == cur_rec_host ) {
			// i can lead
			d_cocklebur->setCurNodeMode( LEADERING );
			d_cocklebur->ack_unlock();
		    } else {
			//someone win my vote
			continue;
		    }
		
		    set< string >::iterator it = t_locked_hosts.begin();
		    for( ; it != t_locked_hosts.end(); it++ ) {
			ele_client.sendMessage( "0", *it );// 0 lead all
		    }
		    break;

		} else {
		    // i cant lead
		    continue;
		}
	    } else {
		// wait to be leadered.
		cout<<"_DEBUG: Waiting leader ( "<<d_voter.rec_host<<" ) sending ack packet."<<endl;
		int time_out_cnt = 0;
		while( d_cocklebur->getCurNodeMode() == LOOKING && time_out_cnt <= 10 ) {
		    sleep(1);
		    ++ time_out_cnt;
		}
		break;
	    }
	}//if
    }//while
}

