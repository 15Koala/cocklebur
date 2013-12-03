#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <map>
#include <string>
using namespace std;

//   Configuration demo
//   Configuration::init("test.xml");
//   map<string,string> a = Configuration::getConfigMapping();
//   cout<<Configuration::get("heartbeat.server.port")<<endl;
//   cout<<Configuration::get("heartbeat.server.port1")<<endl;

class Configuration {
private:
    static map<string,string> d_configMap;//< config mapping 
    Configuration();
    Configuration(Configuration & );
    static void trim(string & s, char ch);
public:
    static map<string,string> & getConfigMapping(); //< return a map
    static void init(const string & );//< initial configuration
    static string get(const string & key);
    static void set(const string & key, const string & value);
};
#endif
