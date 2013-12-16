#ifndef _UTILS_H
#define _UTILS_H
#include <vector>
#include <string>

class DataTree;
class LogEntry;
using namespace std;
// use case
/*
    vector<string> vec_res;
    vector<string> vec_res1;
    string s = ",1111,222,333,4,,,";
    str_split(vec_res, s, ',');
    str_split(vec_res1, s, ',', true);
    vector<string>::iterator it = vec_res.begin();
    for(; it != vec_res.end(); it++){
	cout<<*it<<endl;
    }
    cout<<"------"<<endl;
    it = vec_res1.begin();
    for(; it != vec_res1.end(); it++){
	cout<<*it<<endl;
    }
*/
void str_split(vector<string> & vec, const string & s, char sep);
void str_split(vector<string> & vec, const string & s, char sep, bool ignore_space);

void xid_to_hex_string(string & s, long xid);
long xid_high_32_add(long xid, long a);
long xid_low_32_add(long xid, long a);
int xid_get_high_32(long xid);
int xid_get_low_32(long xid);

long get_cur_timestamp();

string get_seq_encode( const DataTree & ts);
bool get_seq_decode(const string & buff, DataTree * ts);
string get_seq_encode( const LogEntry & ts);
bool get_seq_decode(const string & buff, LogEntry * ts);
bool get_seq_decode(const char * buff, unsigned int size, LogEntry * ts );

long get_file_size( const string & file_name );

#endif
