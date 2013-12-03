/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * the implementation of LeaderElectionServer
 */

#include "LeaderElectionServer.h"
#include "Cocklebur.h"

#include <iostream>

using namespace std;
ElectionServHandler::ElectionServHandler() {
}

Cocklebur * ElectionServHandler::d_cocklebur = 0;
void ElectionServHandler::exchange(Voter& _return, const Voter& voter){
    // read only
    d_cocklebur->getVoter( _return );
    switch ( _return.cur_node_mode ) {

    case LOOKING:
	if( d_cocklebur->ack_get_lock() ) {
	    _return.rec_host = "";
	}
	break;
    case FOLLWING:
	break;
    case LEADERING:
	break;
    }
    
}

bool ElectionServHandler::isLargest(const Voter & voter) {

    if( voter.logical_clock != d_cocklebur->getLogicalClock() ) return false;
    
    if( voter.xid > d_cocklebur->getXid() ){
	bool locked =  d_cocklebur->ack_lock();
	if( locked ) 
	    d_cocklebur->updateHostStat( &voter, false );
	return locked;

    } else if( voter.xid == d_cocklebur->getXid() ) {
	string rec_host_name;
	d_cocklebur->getRecHostName(rec_host_name);

	if( voter.my_host_name >= rec_host_name ) {
	    bool locked = d_cocklebur->ack_lock();
	    if( locked )
		d_cocklebur->updateHostStat( &voter, false );
	    return locked;
	}
	else return false;

    } else return false;
}

void ElectionServHandler::sendMessage(const std::string& message){

    cout<<"_DEBUG: Got message "<<message<<endl;
    if( message == "0" && d_cocklebur->getCurNodeMode() == LOOKING ) {
	d_cocklebur->setCurNodeMode( FOLLWING );
	cout<<"_DEBUG: Mode turn to FOLLWING "<<d_cocklebur->getCurNodeMode()<<endl;
    } else if( message == "1" ){
    }
}

void * startLeaderElectionServer(void * p) {

    pArg p_arg = reinterpret_cast< pArg >(p);
    int port = p_arg->s_port;
    Cocklebur * c = p_arg->s_cocklebur;
    ElectionServHandler::d_cocklebur = c;

    shared_ptr<ElectionServHandler> handler(new ElectionServHandler());
    shared_ptr<TProcessor> processor(new ElectionServProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    cout<<"_DEBUG: election server started listen at port "<<port<<endl;

    delete p_arg;

    server.serve();

    return (void *)0;
}
