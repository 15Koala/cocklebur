#include "Leader.h"
#include "utils.h"
#include "Cocklebur.h"

Leader::Leader( Cocklebur * cocklebur ) {
    d_cocklebur = cocklebur;
}

void Leader::reset() {

    pthread_rwlock_destroy( &m_lease_mapping );
    pthread_rwlock_init( &m_lease_mapping, NULL );

    d_stat_stop = false;
    d_lease_mapping.clear();
    set< string > known_hosts;
    d_cocklebur->getKnownHosts( known_hosts );
    set< string >::iterator it = known_hosts.begin();
    for( ; it != known_hosts.end(); ++it ) {
	d_lease_mapping[*it] = get_cur_timestamp();
    }

}

Leader::~Leader() {
    pthread_rwlock_destroy( &m_lease_mapping );
}

void Leader::leadering() {

    long cur_timestamp = 0;
    unsigned int alive_hosts_num = 0;

    map<string, int> conf_host_list = d_cocklebur->getConfHostList();
    int num_hosts = conf_host_list.size();
    int num_major = num_hosts/2 + 1;

    int flag = 0;

    while( !d_stat_stop ) {
	//cheching lease
	cur_timestamp = get_cur_timestamp();
	pthread_rwlock_wrlock( &m_lease_mapping );
	alive_hosts_num = d_lease_mapping.size();
	if( alive_hosts_num < num_major ) {
	    cout<<"_DEBUG: Lose the majority of followers, turn mode to looking"<<endl;
	    flag = 1;
	    d_cocklebur->setCurNodeMode( LOOKING );
	    break;
	}
	map< string, long >::iterator it = d_lease_mapping.begin();
	for( ; it != d_lease_mapping.end(); ++it ) {
	    if( it->first == d_cocklebur->getConfMyIp() ) continue;
	    cout<<"_DEBUG: Check "<<it->first<<"'s lease, "<<10000-(cur_timestamp-it->second)<<"ms left."<<endl;
	    if( cur_timestamp - it->second > 10000 ) d_lease_mapping.erase(it);
	}
	pthread_rwlock_unlock( &m_lease_mapping );
	sleep(1);
    }

    if( flag ) pthread_rwlock_unlock( &m_lease_mapping );
}

void Leader::updateLease( const string & follower_name ) {
   pthread_rwlock_wrlock( &m_lease_mapping );
   d_lease_mapping[follower_name] = get_cur_timestamp();
   pthread_rwlock_unlock( &m_lease_mapping );
   cout<<"_DEBUG: Updated lease for "<<follower_name<<" ts "<<d_lease_mapping[follower_name]<<endl;
}

void Leader::getMessage( HBMSG & _return ) {
    _return.xid = d_cocklebur->getXid();
    _return.my_host_name = d_cocklebur->getConfMyIp();
    _return.cur_node_mode = d_cocklebur->getCurNodeMode();
}

void Leader::syncData(){}
