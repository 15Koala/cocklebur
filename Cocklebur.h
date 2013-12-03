/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * defination of Cocklebur
 */
#ifndef _COCKLEBUR_H
#define _COCKLEBUR_H

#include <set>
#include "gen-cpp/ElectionServ.h"
#include "gen-cpp/CockleburServ.h"
#include "cocklebur_constance.h"
#include "Configuration.h"
using namespace std;
class Cocklebur {
private:
    /*
     * struct HostStat {
     *  1: i64 xid,
     *  2: set<string> known_hosts,
     *  3: set<string> heard_from,
     *  4: set<string> alive_hosts,
     *  5: string my_host_name,
     *  6: string rec_host,
     *  }
     */
    HostStat d_host_stat;
    /*
     * node mode
     * 1 LOOKING, 2 FOLLOWING, 3 LEADERING
     */
    CockleburMode d_cur_node_mode; //< from snapshot

    /// immutable configuration
    map<string, string> d_conf;//< a reference of configure mapping
    string d_conf_data_dir;//< where snapshot put
    map<string, int> d_conf_map_host_port; //< Host access list from config file
    string d_conf_my_ip;// the hostname of the instance
    int d_conf_my_port;// Cocklebur server port (election server port is d_conf_my_port+1)

    Cocklebur(){}
    void initial_in_memory();// the first start
    void initial_in_file();// when it has snapshots

    void startCockServer( Leader * ); 
    void startCockElectionServer();
    pthread_t m_id_cock_server;//< CockServer thread id
    pthread_t m_id_cock_election_server;//< CockLeaderElection server thread id
    pthread_t m_id_leader_server;//< Cocklebur Server thread id (leader server)

    // locks
    pthread_rwlock_t m_host_stat;//< rw lock for host stat
    pthread_rwlock_t m_cur_node_mode;//< rw lock for current node mode
    pthread_rwlock_t m_ack_lock;

    bool m_server_ack_lock; //< 
public:

    Cocklebur(map<string,string> & map_conf);
    ~Cocklebur();

    /// toString functions.
    void printCockleburInfo();

    /// getters and setters.
    const HostStat & getHostStat() { return d_host_stat; }
    /// to update list: xid, known_hosts, heard_from, alive_hosts, rec_host
    /// the target is dead if the _vote is null, then we will remove it from alive_hosts.
    void updateHostStat( const Voter * _voter, bool use_lock );
    const CockleburMode getCurNodeMode() {
	pthread_rwlock_rdlock( &m_cur_node_mode );
	CockleburMode x =  d_cur_node_mode;
	pthread_rwlock_unlock( &m_cur_node_mode );
	return x;
    }
    void setCurNodeMode( CockleburMode cur_node_mode );
    void getVoter(Voter & _voter); // make a voter to show others
    
    bool ack_lock() {
	bool x;
	pthread_rwlock_wrlock( &m_ack_lock );
	if( m_server_ack_lock ) x = false;
	else {
	    m_server_ack_lock = true;
	    x = true;
	}
	pthread_rwlock_unlock( &m_ack_lock );
	return x;
    }
    void ack_unlock() {
	pthread_rwlock_wrlock( &m_ack_lock );
	m_server_ack_lock = false;
	pthread_rwlock_unlock( &m_ack_lock );
    }
    bool ack_get_lock() {
	pthread_rwlock_rdlock( &m_ack_lock );
	bool x = m_server_ack_lock;
	pthread_rwlock_unlock( &m_ack_lock );
	return x;
    }

    long getXid(){ 
	pthread_rwlock_rdlock( &m_host_stat );
	long x = d_host_stat.xid;
	pthread_rwlock_unlock( &m_host_stat );
	return x;
    }
    void setXid(long xid){
	pthread_rwlock_wrlock( &m_host_stat );
	d_host_stat.xid = xid;
	pthread_rwlock_unlock( &m_host_stat );
    }
    long getLogicalClock() { 
	pthread_rwlock_rdlock( &m_host_stat );
	long x = d_host_stat.logical_clock;
	pthread_rwlock_unlock( &m_host_stat );
	return x;
    }
    void setLogicalClock(long l) { 
	pthread_rwlock_wrlock( &m_host_stat );
	d_host_stat.logical_clock = l;
	pthread_rwlock_unlock( &m_host_stat );
    }
    void getKnownHosts( set< string > & known_hosts ) { 
	pthread_rwlock_rdlock( &m_host_stat );
	known_hosts = d_host_stat.known_hosts; 
	pthread_rwlock_unlock( &m_host_stat );
    }
    void getheardFrom( set< string > & heard_from ) { 
	pthread_rwlock_rdlock( &m_host_stat );
	heard_from = d_host_stat.heard_from;
	pthread_rwlock_unlock( &m_host_stat );
    }
    void getAliveHosts( set< string > & alive_hosts ) { 
	pthread_rwlock_rdlock( &m_host_stat );
        alive_hosts = d_host_stat.alive_hosts; 
	pthread_rwlock_unlock( &m_host_stat );
    }
    void getMyHostName( string & my_host_name ) { 
	my_host_name = d_conf_my_ip;
    }
    void getRecHostName( string & rec_host ){ 
	pthread_rwlock_rdlock( &m_host_stat );
	rec_host = d_host_stat.rec_host; 
	pthread_rwlock_unlock( &m_host_stat );
    }
    void setRecHostName( const string & rec_host ){ 
	pthread_rwlock_rdlock( &m_host_stat );
	d_host_stat.rec_host = rec_host;
	pthread_rwlock_unlock( &m_host_stat );
    }

    const map<string, string> & getConf(){ return d_conf; }
    const string & getConfDataDir(){ return d_conf_data_dir; }
    const map<string,int> & getConfHostList(){ return d_conf_map_host_port; }
    const string & getConfMyIp(){ return d_conf_my_ip; }
    int getConfMyPort(){ return d_conf_my_port; }

    void resetHostStat();

    //  start servers and main process.
    void run();
    // main process of a instance of cocklebur. the last process.
    void process();
};
#endif
