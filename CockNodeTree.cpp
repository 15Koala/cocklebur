#include "CockNodeTree.h"

#include <climits>
#include <fstream>
#include <sstream>

#include "gen-cpp/data_holder_types.h"
#include "utils_time.h"
#include "utils.h"
#include "CLogFile.h"
using namespace std;

CockNodeTree::CockNodeTree( const string & snapshot_dir_path ) {

    //read snapshot content
    long file_size = get_file_size( snapshot_dir_path );
    char * str_snapshot = new char[static_cast<int>(file_size)];
    ifstream fin( snapshot_dir_path.c_str(), ios::in ); 
    fin.read( str_snapshot, file_size );
    fin.close();
    //decode snapshot
    d_data_tree = new DataTree; 
    get_seq_decode( str_snapshot, static_cast<int>(file_size), d_data_tree );
    delete str_snapshot;
    //initial xid lock
    pthread_rwlock_init( &m_xid_lock, NULL );
    // read log, rebuild DataTree
    string str_hex_xid;
    xid_to_hex_string( str_hex_xid, d_data_tree->xid );
    string log_file_name = "./data/" +  str_hex_xid + ".clog";
    d_log_file = new CLogFile( log_file_name );
    vector< LogEntry > log_entries;
    // get log entries.
    d_log_file->scanLog( d_data_tree->xid + 1, LONG_MAX, log_entries );
    // replay logs to rebuild datatree
    replayLogs( log_entries );
}

CockNodeTree::CockNodeTree( long xid ) {
    d_data_tree = new DataTree; 
    d_data_tree->xid = xid;

    // root node
    Node node;
    node.path = "/";
    node.data = "";
    node.data_version = 0;
    node.parent = "";
    node.ctime = CCalendar::getCurTime();
    node.mtime = node.ctime;
    d_data_tree->NodeMap["/"] = node;

    pthread_rwlock_init( &m_xid_lock, NULL );

    string str_hex_xid;
    xid_to_hex_string( str_hex_xid, d_data_tree->xid );
    // snapshot file for data tree
    string snap_file_name = "./data/" + str_hex_xid + ".csnapshot";
    this->snapShot( snap_file_name );
    // log file for snapshot
    string log_file_name = "./data/" + str_hex_xid + ".clog";
    d_log_file = new CLogFile( log_file_name, d_data_tree->xid );
}

CockNodeTree::~CockNodeTree() {
    delete d_data_tree;
    delete d_log_file;
    pthread_rwlock_destroy( &m_xid_lock );
}

long CockNodeTree::xid_plus() {
    long xid = 0;
    pthread_rwlock_wrlock( &m_xid_lock );
    d_data_tree->xid = xid_low_32_add( d_data_tree->xid, 1 );
    xid = d_data_tree->xid;
    pthread_rwlock_unlock( &m_xid_lock );
    return xid;
}

string CockNodeTree::parseParent( const string & t_path ) {

    string path = t_path;
    if( path.rfind('/') == path.length() - 1 ) path = path.substr(0, path.length() -1 ); 

    string s = "/";
    int last_sep = 0;
    if( (last_sep = path.rfind( '/' )) >= 0 ) {
	s = path.substr( 0, last_sep+1 );
	if( "/" == s ) 
	    return s;
	else
	    return s.substr(0, s.length() -1 );
    } else return "";
}
// create a node
// if success , append log
int CockNodeTree::nodeCreate( const string & path ) {
    // create physical path 
    int ret = _create( path );
    if( 0 == ret ) {
	// ACCEPT this operation
	// make log entry
	LogEntry log_entry;
	log_entry.xid = d_data_tree->xid + 1;
	log_entry.ts = CCalendar::getCurTime();
	log_entry.oper = 'c';
	log_entry.content = path;
	log_entry.data = "";
	// append log
	this->logAppend( log_entry );
	// update new node's meta data
	d_data_tree->NodeMap[ path ].ctime = log_entry.ts;
	d_data_tree->NodeMap[ path ].mtime = log_entry.ts;
	// update parent's meta data
	string parent = parseParent( path );
	d_data_tree->NodeMap[ parent ].mtime = log_entry.ts;
	// update DataTree's version.
	xid_plus();
    }
    return ret;
}
// delete a node 
// if success , append log
int CockNodeTree::nodeDelete( const string & path ) {
    // delete physical path
    int ret = _delete( path );
    if( 0 == ret ) {
	// ACCEPT this operation
	// make log entry
	LogEntry log_entry;
	log_entry.xid = d_data_tree->xid + 1;
	log_entry.ts = CCalendar::getCurTime();
	log_entry.oper = 'd';
	log_entry.content = path;
	log_entry.data = "";
	// append log
	this->logAppend( log_entry );
	// update parent's lastmodify
	string parent = parseParent( path );
	d_data_tree->NodeMap[parent].mtime = log_entry.ts;
	// update datatree's version.
	xid_plus();
    }
    return ret;
}
// return 2: path already exists
// return 1: parent not found
// return 0: create successfully
// create physical path without meta data considered
int CockNodeTree::_create( const string & t_path ) {
    string path = t_path;
    if( path.rfind('/') == path.length() - 1 ) path = path.substr(0, path.length() -1 ); 
    string parent = parseParent( path );
    if( !nodeExist( parent ) ) return 1;
    if( nodeExist( path ) ) return 2;
    Node node;
    node.data = "";
    node.data_version = 0;
    node.parent = parent;
    node.ctime = 0;
    node.mtime = 0;
    node.path = path;
    d_data_tree->NodeMap[path] = node;
    d_data_tree->NodeMap[parent].children.insert( path );
    return 0;
}
// return 1: no path to delete
// return 0: delete successfully
// delete physical path without meta data considered
int CockNodeTree::_delete( const string & path ) {
    string parent = parseParent( path );
    if( !nodeExist( path ) ) return 1;
    d_data_tree->NodeMap[parent].children.erase( path );
    set< string >::iterator it = d_data_tree->NodeMap[path].children.begin();
    for( ; it != d_data_tree->NodeMap[path].children.end(); ++it ) {
	_delete( *it );
    }
    d_data_tree->NodeMap.erase( path );
    return 0;
}
bool CockNodeTree::nodeExist( const string & path ) {

    if( "/" == path ) return true;
    
    if( 0 < d_data_tree->NodeMap.count( path ) ) 
	return true;
    else
	return false;
}

string CockNodeTree::getData( const string & path ) {
    
    if( "/" == path || !nodeExist( path ) ) return "";

    return d_data_tree->NodeMap[ path ].data;
}
// set data
// if success, append log
bool CockNodeTree::setData( const string & path, const string & data ) {

    if( "/" == path || !nodeExist( path ) ) return false;
    
    //make log entry
    LogEntry log_entry;
    log_entry.xid = d_data_tree->xid + 1;
    log_entry.ts = CCalendar::getCurTime();
    log_entry.oper = 's';
    log_entry.content = path;
    log_entry.data = data;
    // append log
    this->logAppend( log_entry );

    d_data_tree->NodeMap[ path ].data = data;
    // update lastmodify and data version
    d_data_tree->NodeMap[ path ].data_version ++;
    d_data_tree->NodeMap[ path ].mtime = log_entry.ts;

    // alter datatree successfully, xid++
    xid_plus();

    return true;
}

int CockNodeTree::getChildren( const string & path, set< string > &  _children ) {
    
    if( !nodeExist( path ) ) return 1;

    set< string >::iterator it = d_data_tree->NodeMap[ path ].children.begin();
    for( ; it != d_data_tree->NodeMap[ path ].children.end() ; ++it ) {
	_children.insert( *it );
    }
    return 0;
}
DataTree CockNodeTree::getDataTree( ) {
    return *d_data_tree;
}

void CockNodeTree::replayLogs( const vector < LogEntry > & log_entries ) { /// replay logs.
    
    cout<<"_DEBUG: replaying logs. "<<log_entries.size()<<" entries has been found."<<endl;
    vector<LogEntry>::const_iterator it = log_entries.begin();
    for( ; it != log_entries.end(); ++it ) {
	cout<<"_DEBUG: replaying entry "<<d_log_file->strLogEntry( *it )<<endl;
	replayEntry( *it );
    }
}

int CockNodeTree::replayEntry( const LogEntry & log_entry ) {
    int ret = 1;
    switch( log_entry.oper ) {
    case 'c':
	// create physical path 
	ret = _create( log_entry.content );
	if( 0 == ret ) {
	    // ACCEPT this operation
	    // update new node's meta data
	    d_data_tree->NodeMap[ log_entry.content ].ctime = log_entry.ts;
	    d_data_tree->NodeMap[log_entry.content ].mtime = log_entry.ts;
	    // update parent's meta data
	    string parent = parseParent( log_entry.content );
	    d_data_tree->NodeMap[ parent ].mtime = log_entry.ts;
	    // update DataTree's version.
	    xid_plus();
	}
	break;
    case 'd':
	// delete physical path
	ret = _delete( log_entry.content );
	if( 0 == ret ) {
	    // ACCEPT this operation
	    // update parent's lastmodify
	    string parent = parseParent( log_entry.content );
	    d_data_tree->NodeMap[parent].mtime = log_entry.ts;
	    // update datatree's version.
	    xid_plus();
	}
	break;
    case 's':
	if( "/" == log_entry.content || !nodeExist( log_entry.content ) ) ret = 1;

	d_data_tree->NodeMap[ log_entry.content ].data = log_entry.data;
	// update lastmodify and data version
	d_data_tree->NodeMap[ log_entry.content ].data_version ++;
	d_data_tree->NodeMap[ log_entry.content ].mtime = log_entry.ts;
	// update datatree's version.
	xid_plus();

	ret = 0;
	break;
    }
    return ret;
}
// Backup
void CockNodeTree::snapShot( const string & file_name ) {
    string seq_data_tree;
    seq_data_tree = get_seq_encode( *d_data_tree );
    DataTree *d = new DataTree;
    get_seq_decode(seq_data_tree,d);
    
    ofstream fout( file_name.c_str() , ios::binary);
    fout.write( seq_data_tree.c_str(), seq_data_tree.size() );
    fout.close();
}

void CockNodeTree::logAppend( const LogEntry & log_entry ) {
    d_log_file->appendLog( log_entry );
}

string CockNodeTree::toString() {
    stringstream s;
    s.clear();
    s << "{ name=>'cockNodeTree'" \
	<< ", xid=>" <<this->d_data_tree->xid \
	<< ", nodes=>[ ";
    map< string, Node >::iterator it = this->d_data_tree->NodeMap.begin();
    for( ; it != d_data_tree->NodeMap.end() ; ++it ) {
	s<< "{ path=>'" << it->second.path << "', parent=>'" \
	    << it->second.parent << "', ctime=>"\
	    << it->second.ctime << ", mtime=>"\
	    << it->second.mtime << ", data_version=>" \
	    << it->second.data_version << ", children=>[ " ;
	set< string >::iterator c_it = it->second.children.begin();
	for( ; c_it != it->second.children.end(); ++ c_it ) {
	    s<< *c_it << " ";
	}
	s<<"] } "<<endl;
    }
    s<<"] }";
    return s.str();
}
