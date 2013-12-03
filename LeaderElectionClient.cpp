#include "LeaderElectionClient.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "gen-cpp/ElectionServ.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using boost::shared_ptr;

LeaderElectionClient::LeaderElectionClient( const map< string, int > & conf_hosts ) {

    d_host_port_map = conf_hosts;
}
void LeaderElectionClient::exchange( Voter * _voter, const Voter & myVoter, const string & target_host ) {

   int port = d_host_port_map[target_host] + 1;
   shared_ptr<TSocket> socket(new TSocket(target_host, port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

   try {
       transport->open();
       ElectionServClient client(protocol);
       client.exchange(*_voter, myVoter);
       // target_host is already locked by some one, donesnt matter, exchange next.
       //if( _voter->logical_clock < myVoter.logical_clock && _voter->cur_node_mode == 1 ) _voter = NULL;// a invalid vote
       transport->close();

   } catch( apache::thrift::transport::TTransportException e ) {
   }
}
bool LeaderElectionClient::isLargest( const Voter & voter,  const string & target_host ){

   int port = d_host_port_map[target_host] + 1;
   shared_ptr<TSocket> socket(new TSocket(target_host, port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
   try {

       transport->open();
       ElectionServClient client(protocol);
       bool _return = client.isLargest(voter);
       transport->close();
       return _return;

   }catch( apache::thrift::transport::TTransportException e ) {
       return false;
   }

}
bool LeaderElectionClient::sendMessage( const string & message, const string & target_host ){

   int port = d_host_port_map[target_host] + 1;
   shared_ptr<TSocket> socket(new TSocket(target_host, port));
   shared_ptr<TTransport> transport(new TBufferedTransport(socket));
   shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

   transport->open();
   ElectionServClient client(protocol);
   client.sendMessage(message);
   transport->close();
}
