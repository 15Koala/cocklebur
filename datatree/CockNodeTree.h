/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * an CockNodeTree.h
 */

#ifndef _COCK_NODE_TREE_H
#define _COCK_NODE_TREE_H

#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;

class Node;
class LogEntry;
class DataTree;
class CockNodeTree {

private:
    DataTree * d_data_tree;//< a pointer holder of the CockNodeTree
    
    void readSnapShot(); /// read snapshot file.
    void replayLogs( const vector < LogEntry > & log_entries ); /// replay logs.
    void xid_plus(); // xid ++
    bool checkTree(); /// check the data node tree is the newest or not.

    CockNodeTree();
    CockNodeTree( CockNodeTree & );

    pthread_rwlock_t m_xid_lock;//< xid lock

public:
    string parseParent( const string & path );

    CockNodeTree( long xid );
    CockNodeTree( const string & snapshot_dir_path );
    ~CockNodeTree();

    // CRUD
    int nodeCreate( const string & path );
    int nodeDelete( const string & path );
    bool nodeExist( const string & path );

    string getData( const string & path );
    bool setData( const string & path, const string & data );

    int getChildren( const string & path, set< string >  & _children );
    DataTree getDataTree( );
    vector < LogEntry > getLogEntry( long xid );

    // Backup
    void snapShot( const string & file_name);
    void logAppend( const LogEntry & log_entry , const string & file_name );

    // stat
    string toString();
};

#endif

