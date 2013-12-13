#include "CLogFile.h"

#include <fstream>

#include "CByte.h"
#include "utils.h"
#include "gen-cpp/data_holder_types.h"

using namespace std;
// open an old file
CLogFile::CLogFile( const string & file_name ) {

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

vector< LogEntry > CLogFile::scanLog( long start, long end ) {

}

