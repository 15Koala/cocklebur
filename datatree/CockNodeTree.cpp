#include "CockNodeTree.h"

#include <fstream>
#include <sstream>

#include "../gen-cpp/data_holder_types.h"
#include "../utils_time.h"
#include "../utils.h"
using namespace std;

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

    // data version ++ 
    pthread_rwlock_init( &m_xid_lock, NULL );
    xid_plus();

    string str_hex_xid;
    xid_to_hex_string( str_hex_xid, d_data_tree->xid );
    // snapshot file for data tree
    string snap_file_name = str_hex_xid + ".csnapshot";
    this->snapShot( snap_file_name );
    // log file for snapshot
    string log_file_name = str_hex_xid + ".clog";
}

CockNodeTree::~CockNodeTree() {
    delete d_data_tree;
    pthread_rwlock_destroy( &m_xid_lock );
}

void CockNodeTree::xid_plus() {
    pthread_rwlock_wrlock( &m_xid_lock );
    d_data_tree->xid = xid_low_32_add( d_data_tree->xid, 1 );
    pthread_rwlock_unlock( &m_xid_lock );
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

int CockNodeTree::nodeCreate( const string & t_path ) {
    string path = t_path;
    if( path.rfind('/') == path.length() - 1 ) path = path.substr(0, path.length() -1 ); 

    string parent = parseParent( path );

    if( !nodeExist( parent ) ) return 1;
    if( nodeExist( path ) ) return 2;

    Node node;
    node.data = "";
    node.data_version = 0;
    node.parent = parent;
    node.ctime = CCalendar::getCurTime();
    node.mtime = node.ctime;
    node.path = path;
    d_data_tree->NodeMap[path] = node;
    d_data_tree->NodeMap[parent].children.insert( path );
    
    xid_plus();
    return 0;
}

int CockNodeTree::nodeDelete( const string & path ) {
    string parent = parseParent( path );

    if( !nodeExist( path ) ) return 1;
    d_data_tree->NodeMap[parent].children.erase( path );

    set< string >::iterator it = d_data_tree->NodeMap[path].children.begin();
    for( ; it != d_data_tree->NodeMap[path].children.end(); ++it ) {
	nodeDelete( *it );
    }

    d_data_tree->NodeMap.erase( path );

    xid_plus();
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

bool CockNodeTree::setData( const string & path, const string & data ) {

    if( "/" == path || !nodeExist( path ) ) return false;
    
    d_data_tree->NodeMap[ path ].data = data;
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

vector < LogEntry > CockNodeTree::getLogEntry( long xid ) {
}

// Backup
void CockNodeTree::snapShot( const string & file_name ) {
    string seq_data_tree;
    seq_data_tree = get_seq_encode( *d_data_tree );
    
    ofstream fout( file_name.c_str() , ios::binary);
    fout.write( seq_data_tree.c_str(), seq_data_tree.size() );
    fout.close();
}

void CockNodeTree::logAppend( const LogEntry & log_entry, const string & file_name ) {
    unsigned int entry_size = sizeof( log_entry );
    // log file structure 
    // size | seq data 
    // 4bytes | sizeof 
}

string CockNodeTree::toString() {
    stringstream s;
    s << "->cockNodeTree:" << endl \
	<< " ->xid: " <<this->d_data_tree->xid << endl \
	<< " ->nodes: " << endl ;
    map< string, Node >::iterator it = this->d_data_tree->NodeMap.begin();
    for( ; it != d_data_tree->NodeMap.end() ; ++it ) {
	s<< "  ->" << it->second.path << "," \
	    << it->second.parent << ","\
	    << it->second.ctime << ","\
	    << it->second.mtime << "," \
	    << it->second.data_version << ", [" ;
	set< string >::iterator c_it = it->second.children.begin();
	for( ; c_it != it->second.children.end(); ++ c_it ) {
	    s<< *c_it << " ";
	}
	s<<"]"<<endl;
    }
    return s.str();
}
