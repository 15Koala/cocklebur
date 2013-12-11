/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * an CockNodeTree.h
 */

#ifndef _COCK_NODE_TREE_H
#define _COCK_NODE_TREE_H

class CockNodeTree {

private:
    DataTree * d_data_tree;//< a pointer holder of the CockNodeTree
    
    void readSnapShot(); /// read snapshot file.
    void replayLogs( const vector < LogEntry > & log_entries ); /// replay logs.
    
    bool checkTree(); /// check the data node tree is the newest or not.

    CockNodeTree();
    CockNodeTree(&);

    // tree lock
    pthread_rwlock_t m_data_tree;

public:

    CockNodeTree( long xid );
    CockNodeTree( const string & snapshot_dir_path );
    ~CockNodeTree();

    // CRUD
    int nodeCreate( const string & path, bool isBlocked );
    int nodeDelete( const string & path );
    bool nodeExist( const string & path );

    string getData( const string & path );
    void setData( const string & path, const string & data );

    vector < Node > getChildren( const string & path );
    DataTree getDataTree( const string & path );
    vector < LogEntry > getLogEntry( long xid );

    // Backup
    void snapShot();
    void logAppend( const LogEntry & log_entry);
};

#endif

