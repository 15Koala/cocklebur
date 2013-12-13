#include "CLogFile.h"

#include <fstream>

#include "CByte.h"
#include "utils.h"
#include "gen-cpp/data_holder_types.h"

using namespace std;
// open an old file
CLogFile::CLogFile( const string & file_name ) {

    d_file_name = file_name;
    vector< string > tmp;
    str_split( tmp, file_name, '.' );
    d_start_xid = atol( tmp[0].c_str() );

    fstream fin( file_name.c_str(), ios::binary );
    unsigned int cur_pos = 0;
    unsigned int val_len = 0;
    unsigned int *p_val_len = &val_len;
    long xid = 0;
    long * p_xid = &xid;
    while( !fin.eof() ) {
	cur_pos += 4;
	fin.seekg( cur_pos );
	fin.read( reinterpret_cast< char * >(p_xid), 8 );
	fin.read( reinterpret_cast< char * >(p_val_len), 4 );
	cur_pos += val_len;
	fin.seekg( cur_pos );
    }

    d_cur_xid = xid;
    fin.close();

}
// open a new file
CLogFile::CLogFile( const string & file_name, long xid ) {

    d_file_name = file_name;
    d_cur_xid = xid;
    d_start_xid = xid;

    ofstream fout( file_name.c_str() , ios::binary);
    fout.close();
}

int CLogFile::appendLog( const LogEntry & log_entry ) {

    std::size_t key_length = sizeof(log_entry.xid);
    string seq_val = get_seq_encode( log_entry );
    std::size_t val_length = seq_val.size();
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
    
    fstream fin( d_file_name.c_str(), ios::binary );
    unsigned int cur_pos = 0;
    unsigned int val_len = 0;
    unsigned int *p_val_len = &val_len;
    long xid = 0;
    long * p_xid = &xid;

    char * cur_seq_data = new char[MAX_DATA_BUFFER];
    LogEntry log_entry;
    LogEntry * p_log_entry = &log_entry;
    while ( !fin.eof() ) {
	cur_pos += 4;
	fin.seekg( cur_pos );
	fin.read( reinterpret_cast< char * >(p_xid), 8 );
	fin.read( reinterpret_cast< char * >(p_val_len), 4 );
	fin.read( cur_seq_data, val_len );
	get_seq_decode( cur_seq_data, val_len, p_log_entry );
	_log_entries.push_back( log_entry );
    }

    fin.close();
}

