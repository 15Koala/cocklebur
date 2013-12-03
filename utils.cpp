#include "utils.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <sys/time.h>
#include <sys/vfs.h>
using namespace std;
void str_split(vector<string> & vec, const string & s, char sep){
    int last = 0;
    for(int i=0; i <= s.size(); i++){
	if(s[i] == sep || i == s.size() ){
	    vec.push_back(s.substr(last, i-last));
	    last = i+1;
	}
    }
}
void str_split(vector<string> & vec, const string & s, char sep, bool ignore_space){
    if(!ignore_space)
	str_split(vec,s,sep);
    else{
	int last = 0;
	for(int i=0; i <= s.size(); i++){
	    if(s[i] == sep || i == s.size() ){
		string tmp = s.substr(last, i-last);
		if(tmp != "" ) vec.push_back(tmp);
		last = i+1;
	    }
	}
    }
}
void xid_to_hex_string(string & s, long xid){
  stringstream ss;
  ss.clear();
  ss<<hex<<xid;
  ss>>s;
  if(s.size()<16){
      string tmp(16-s.size(),'0');
      s = tmp + s;
  }
}
long xid_high_32_add(long xid, long a){
    long tmp = a << 32;
    cout<<xid+tmp<<endl;
    return xid + tmp;
}
long xid_low_32_add(long xid, long a){
    return xid + a;
}
int xid_get_high_32(long xid){
    return xid >> 32;
}
int xid_get_low_32(long xid){
    long a  = xid >> 32;
    a = a << 32;
    return xid - a;
}
long get_cur_timestamp(){
     struct timeval tv;
     gettimeofday(&tv,NULL);
     return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

