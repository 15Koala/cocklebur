/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * NameSpace.h
 * maintain a memory file namespace.
 */

#ifndef _NAME_SPACE_H
#define _NAME_SPACE_H

#include <pthread.h>
#include <string>
#include <queue>
#include <map>

using namespace std;

class LogEntry;
class CockNodeTree;
class NameControl {

private:

    bool isReady;//< default is false
    CockNodeTree * d_cock_node_tree;//< mem namespace

    queue < LogEntry > d_operation_queue;//< operation queue
    int cur_seq_num;
    pthread_rwlock_t m_operation_queue;//< queue lock
    map< int, pthread_rwlock_t> notify_map;//< log_seq => wait_lock
public:

    /// scan data dir and find out who is the lastest.
    string findLastestLogName( const string & data_dir_name );
    NameControl();
    ~NameControl();
    // 1. find lastest log file.
    // 2. load snapshot and log.
    // 3. replay logs and rebuild datatree.
    // 4. disable server.
    // 5. find out sequence number of operations.
    // 6. return current xid to Cocklebur
    long init( const string & data_dir_name );

    int putEntry( const LogEntry & );
    LogEntry getEntry();

};

#endif
