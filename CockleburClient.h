/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * CockleburClient.h
 */

#ifndef _COCKLEBUR_CLIENT_H
#define _COCKLEBUR_CLIENT_H

#include <map>
#include <string>
using namespace std;

class HBMSG;
class HostStat;
class CockleburClient {

private:

    map<string, int> d_host_port_map;//< host name => port mapping
    explicit CockleburClient();
    explicit CockleburClient(CockleburClient &);

public:

    explicit CockleburClient( const map<string, int> & conf_hosts );
    /*
     * @para const string & target_host: target host.
     * @return HostStat & _return.
     * @return if connection failed, false
     */
    bool getHostStat(HostStat & _return, const string & target_host);
    /*
     * @return string & _return.
     * @return if connection failed, false
     */
    bool ping( std::string& _return, const std::string& message, const string & target_host );
    bool ping2( HBMSG & _return, const HBMSG & message, const string & target_host );
    
};

#endif
