#include "NameControl.h"
#include "gen-cpp/DataServ.h"

#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "CockNodeTree.h"
#include "utils.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

// NameSpaceServer 
// wrapper NameSpace as a server. 
// to accept request from clients.
class NameSpaceServer : virtual public DataServIf {

private:
    NameControl * name_control;
public:
    NameSpaceServer( NameControl * ns ) {
	name_control = ns;
    }

    int32_t nodeCreate(const std::string& path, const bool isBlocked) {
	// Your implementation goes here
	printf("nodeCreate\n");
    }

    int32_t nodeDelete(const std::string& path) {
	// Your implementation goes here
	printf("nodeDelete\n");
    }

    bool nodeExist(const std::string& path) {
	// Your implementation goes here
	printf("nodeExist\n");
    }

    void getData(std::string& _return, const std::string& path) {
	// Your implementation goes here
	printf("getData\n");
    }

    void setData(const std::string& path, const std::string& data) {
	// Your implementation goes here
	printf("setData\n");
    }

    void getChildren(std::vector<Node> & _return, const std::string& path) {
	// Your implementation goes here
	printf("getChildren\n");
    }

    void getDataTree(DataTree& _return) {
	// Your implementation goes here
	printf("getDataTree\n");
    }

    void getLogEntryList(std::vector<LogEntry> & _return, const int64_t xid) {
	// Your implementation goes here
	printf("getLogEntryList\n");
    }

};

int startNameSpaceServer(void * p) {
    NameControl * ns;
    int port = 9090;
    shared_ptr<NameSpaceServer> handler(new NameSpaceServer(ns));
    shared_ptr<TProcessor> processor(new DataServProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}

// NameControl functions
NameControl::NameControl(): isReady(false) {

}

NameControl::~NameControl() {

    delete d_cock_node_tree;
    pthread_rwlock_destroy( &m_operation_queue );
}

int NameControl::putEntry( const LogEntry & log_entry ) {

}

LogEntry NameControl::getEntry() {

}

long NameControl::init( const string & data_dir_name ) {
    // if there is no logs and matched snapshots. we should initial
    // datatree automatically. Touch snapshots and create a log file
    // for datatree.
    if( access( data_dir_name.c_str(), 0 ) != 0 ) 
	mkdir(data_dir_name.c_str(), 0775);

    string max_prefix = findLastestLogName( data_dir_name );
    if( max_prefix != "" ) {
	cout<<"_DEBUG: find lastest cache files with prefix "<<max_prefix<<endl;
       // we have some data cached, so rebuld datatree.	
	d_cock_node_tree = new CockNodeTree( max_prefix + ".csnapshot" );
    } else {
	cout<<"_DEBUG: find nothing cached in dictory "<<data_dir_name<<endl;
	// nothing cached in data dir
	d_cock_node_tree = new CockNodeTree(0);
	d_cock_node_tree->nodeCreate("/root");
    }
}

string NameControl::findLastestLogName( const string & data_dir_name ) {
    DIR * dir;
    struct dirent *s_dir;
    dir=opendir( data_dir_name.c_str() );
    string cur_latest_file_name;
    while((s_dir = readdir(dir)) != NULL)
    {
	if((strcmp(s_dir->d_name,".")==0) || (strcmp(s_dir->d_name,"..")==0)){
	    continue;
	}
	string t_s(s_dir->d_name);
	if( t_s > cur_latest_file_name ) {
	    cur_latest_file_name = s_dir->d_name;
	}
    }
    vector<string> tmp;
    str_split(tmp,cur_latest_file_name,'.');
    return tmp[0];
}
