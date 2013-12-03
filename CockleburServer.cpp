#include "CockleburServer.h"
#include "Leader.h"
#include "Cocklebur.h"

CockleburServHandler::CockleburServHandler(Leader * leader) {
    d_leader = leader;
}

void CockleburServHandler::getHostStat(HostStat& _return){

}

void CockleburServHandler::ping(std::string& _return, const std::string& message){

}

void CockleburServHandler::ping2( HBMSG & _return, const HBMSG & message){
    cout<<"_DEBUG: Got message < "<<message.my_host_name<<", "<<message.xid<<", "<<message.cur_node_mode<<" >"<<endl;
    d_leader->getMessage( _return );
    if( message.cur_node_mode == FOLLWING && _return.cur_node_mode == LEADERING ) {
	d_leader->updateLease( message.my_host_name );
    }
    cout<<"_DEBUG: Return message < "<<_return.my_host_name<<", "<<_return.xid<<", "<<_return.cur_node_mode<<" >"<<endl;
}

void *startCockleburServer(void * p) {

    lArg l_arg = reinterpret_cast< lArg > (p);
    int port = l_arg->s_port;
    Leader * l = l_arg->s_leader;

    shared_ptr<CockleburServHandler> handler(new CockleburServHandler(l));
    shared_ptr<TProcessor> processor(new CockleburServProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    cout<<"_DEBUG: Leader server started listen at port "<<port<<endl;
    delete l_arg;
    server.serve();

}
