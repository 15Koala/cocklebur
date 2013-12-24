/**
 * Copyright 2013-12-13 koala.
 * author koala
 * qq: 515539648
 * CLogFile.h
 */

#ifndef _C_LOGFILE_H
#define _C_LOGFILE_H

// the max size of LogEntry sequence data
#define MAX_DATA_BUFFER 1024

#include <string>
#include <vector>

using namespace std;

class LogEntry;
class CLogFile {
private:

    /*
     *   Cock Log File Logical Picture
     *   
     *   file_name start_xid.log
     *   | 4bytes | 8bytes | 4 bytes(A) | A bytes  |
     *   | keylen | key    | vallen     | seq data |
     *   xid=>LogEntry
     */

    CLogFile();
    CLogFile( CLogFile & );

    string d_file_name;//< file name 
    long d_cur_xid;//< log entry xid
    long d_start_xid;//< xid of the first log entry
    
public:

    explicit CLogFile( const string & file_name );/// open an old file.
    CLogFile( const string & file_name, long xid );/// open a new file.

    // return 
    // 0 success, 1 file_not_found, 2 other
    int appendLog( const LogEntry & log_entry );
    void scanLog( long start, long end, vector< LogEntry > & _log_entries );
    void printLogEntries( const vector< LogEntry > & _log_entries );
    string strLogEntry( const LogEntry & );

};

#endif

