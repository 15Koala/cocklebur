/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * client test
 */
#include "../gen-cpp/ElectionServ.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <iostream>
#include "../cocklebur_constance.h"
using namespace std;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using boost::shared_ptr;
int main(int argc, char **argv) {
    int port = 10011;
    shared_ptr<TSocket> socket(new TSocket("localhost", port));
    shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    transport->open();
    ElectionServClient client(protocol);
    Voter _return;
    Voter _return2;
    client.exchange(_return, _return2);
    cout<<_return.xid<<endl;
    cout<<_return.my_host_name<<endl;
    cout<<_return.rec_host<<endl;
    transport->close();
    return 0;
}
