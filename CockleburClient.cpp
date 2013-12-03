#include "CockleburClient.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "gen-cpp/CockleburServ.h"
#include "utils_time.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using boost::shared_ptr;

CockleburClient::CockleburClient( const map<string, int> & conf_hosts ) {

    d_host_port_map = conf_hosts;
}

bool CockleburClient::getHostStat(HostStat& _return, const string & target_host) {

   int port = d_host_port_map[target_host];
   shared_ptr<TSocket> socket(new TSocket(target_host, port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

   try {

       transport->open();
       CockleburServClient client(protocol);
       client.getHostStat(_return);
       transport->close();
       return true;

   } catch( apache::thrift::transport::TTransportException e ) {
       return false;
   }

}

bool CockleburClient::ping(std::string& _return, const std::string& message, const string & target_host) {

   int port = d_host_port_map[target_host] + 2;
   shared_ptr<TSocket> socket(new TSocket(target_host, port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

   try {

       transport->open();
       CockleburServClient client(protocol);
       client.ping(_return, message);
       transport->close();
       return true;

   } catch( apache::thrift::transport::TTransportException e ) {
       return false;
   }

}

bool CockleburClient::ping2( HBMSG & _return, const HBMSG & message, const string & target_host ) {
    
    if( target_host == message.my_host_name ) {
	cout<<"_DEBUG: Avoid ping myself."<<endl;
	return false;
    }

    int port = d_host_port_map[target_host] + 2;
    shared_ptr<TSocket> socket(new TSocket(target_host, port));
    shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    try {

	transport->open();
	CockleburServClient client(protocol);
	cout<<"_DEBUG: Ping "<<target_host<<" with message < "<<message.my_host_name<<", "<<message.xid<<", "<<message.cur_node_mode<<" >"<<endl;
	client.ping2(_return, message);
	transport->close();
	cout<<"_DEBUG: Got message < "<<_return.my_host_name<<", "<<_return.xid<<", "<<_return.cur_node_mode<<" >"<<endl;
	return true;

    } catch( apache::thrift::transport::TTransportException e ) {
	return false;
    }

}

