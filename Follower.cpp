#include "Follower.h"

#include "utils.h"
#include "CockleburClient.h"
#include "Cocklebur.h"
#include "gen-cpp/leader_election_types.h"

Follower::Follower( Cocklebur * cocklebur ) {

    d_cocklebur = cocklebur;
    d_cock_client = new CockleburClient( d_cocklebur->getConfHostList() );
    d_stat_stop = false;
}

Follower::~Follower() {
    delete d_cock_client;
}

void Follower::following() {

    string leader;
    d_cocklebur->getRecHostName( leader );
    long start_timestamp = get_cur_timestamp();

    while( !d_stat_stop ) {
	// heart beat
	cout<<"_DEBUG: Follower "<<d_cocklebur->getConfMyIp()<<" keep alive for "<<\
	    (get_cur_timestamp()-start_timestamp)/1000<<"s."<<endl;
	int retry_num = 0;
	HBMSG message, _message;
	message.xid = d_cocklebur->getXid();
	message.my_host_name = d_cocklebur->getConfMyIp();
	message.cur_node_mode = d_cocklebur->getCurNodeMode();
	while( !d_cock_client->ping2( _message, message, leader ) ){
	    if( ++retry_num > Configuration::getLong("cocklebur.follow.timeout") / 1000 ) {
		_message.my_host_name = "";
		break;
	    }
	    sleep(1);
	}
	
	if( _message.cur_node_mode != LEADERING || _message.my_host_name == "" ) {
	    d_cocklebur->setCurNodeMode( LOOKING );
	    cout<<"_DEBUG: "<<leader<<" is not leader any more, turn to looking mode."<<endl;
	    break;
	}

	if( _message.xid > message.xid ) {
	    // pull data
	}
	else if( _message.xid < message.xid ) {
	    //push data
	} else {
	    cout<<"_DEBUG: Got xid "<<_message.xid<<", no data update. "<<endl;

	}

	sleep( Configuration::getLong("cocklebur.follow.heatbeat.interval") / 1000 );
    }//while
}
void Follower::syncData(){}
