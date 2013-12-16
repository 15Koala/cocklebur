#include "CLogFile.h"
#include <fstream>

#include "CByte.h"
#include "utils.h"
#include "gen-cpp/data_holder_types.h"

using namespace std;
// open an old file
CLogFile::CLogFile( const string & file_name ) {

    cout<<"_DEBUG: open log file "<<file_name<<"."<<endl;
    d_file_name = file_name;
    vector< string > tmp;
    str_split( tmp, file_name, '.' );
    d_start_xid = atol( tmp[0].c_str() );

    fstream fin( file_name.c_str(), ios::in );
    char buff[MAX_DATA_BUFFER];
    unsigned int val_len = 0;
    long cur_xid = 0;
    unsigned int key_len = 0;
    
    while ( fin.tellg() < get_file_size(file_name) ) {

	fin.read( buff, 16 );
	key_len = CByte::getUInt( buff, 4 );
	cur_xid = CByte::getLong( buff + 4, 8 );
	val_len = CByte::getUInt( buff + 12, 4 );
	fin.read( buff, val_len );
    }
    d_cur_xid = cur_xid;
    fin.close();

}
// open a new file
CLogFile::CLogFile( const string & file_name, long xid ) {

    cout<<"_DEBUG: new a log file "<<file_name<<" with xid "<<xid<<endl;
    d_file_name = file_name;
    d_cur_xid = xid;
    d_start_xid = xid;

    ofstream fout( file_name.c_str() , ios::binary);
    fout.close();
}

int CLogFile::appendLog( const LogEntry & log_entry ) {

    if( log_entry.oper == 's' )
	cout<<"_DEBUG: append log ( xid:"<<log_entry.xid<<", ts:"<<log_entry.ts<<", oper:s, content:"<<log_entry.content.size()<<" )"<<endl;
    else
	cout<<"_DEBUG: append log ( xid:"<<log_entry.xid<<", ts:"<<log_entry.ts<<", oper:"<<log_entry.oper<<", data_size:"<<log_entry.content<<" )"<<endl;
	
    unsigned int key_length = sizeof(log_entry.xid);
    string seq_val = get_seq_encode( log_entry );
    unsigned int val_length = seq_val.size();
    string seq_key_len = CByte::toBytes( key_length );
    string seq_val_len = CByte::toBytes( val_length );
    string seq_key = CByte::toBytes( log_entry.xid );

    ofstream fout( d_file_name.c_str(), ios::app );

    fout.write( seq_key_len.c_str(), seq_key_len.size() );
    fout.write( seq_key.c_str(), seq_key.size() );
    fout.write( seq_val_len.c_str(), seq_val_len.size() );
    fout.write( seq_val.c_str(), seq_val.size() );

    fout.close();
}

void CLogFile::scanLog( long start, long end, vector< LogEntry > & _log_entries ) {

    cout<<"_DEBUG: scan logs from "<<start<<" to "<<end<<"."<<endl;
    
    fstream fin( d_file_name.c_str(), ios::in );
    unsigned int key_len = 0;
    unsigned int val_len = 0;
    long cur_xid = 0;

    char buff[MAX_DATA_BUFFER];
    LogEntry log_entry;
    LogEntry * p_log_entry = &log_entry;

    while ( fin.tellg() < get_file_size(d_file_name) ) {

	fin.read( buff, 16 );
	key_len = CByte::getUInt( buff, 4 );
	cur_xid = CByte::getLong( buff + 4, 8 );
	val_len = CByte::getUInt( buff + 12, 4 );
	fin.read( buff, val_len );
	get_seq_decode( buff, val_len, p_log_entry );

	if(cur_xid >= start && cur_xid < end )
	    _log_entries.push_back( log_entry );
    }

    fin.close();
}

void CLogFile::printLogEntries( const vector< LogEntry > & _log_entries ) {
    
    cout<<"{ name=>'LogEntries', content=>[ ";
    vector< LogEntry >::const_iterator it = _log_entries.begin();
    for( ; it != _log_entries.end(); ++it ) {
	cout<<"{ xid=>"<<it->xid<<", ts=>"<<it->ts<<", oper=>'"<<it->oper<<"', data_size=>"<<it->content.size()<<" }, ";
    }
    cout<<" ] }"<<endl;
}
