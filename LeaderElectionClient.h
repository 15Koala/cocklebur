/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * LeaderElectionClient.h
 */

#ifndef _LEADERE_LECTION_CLIENT_H
#define _LEADERE_LECTION_CLIENT_H

#include <string>
#include <map>
using namespace std;
class Voter;
class Election;
class LeaderElectionClient {
private:

    map<string, int> d_host_port_map;//< host name => port mapping
    explicit LeaderElectionClient();
    explicit LeaderElectionClient(LeaderElectionClient &);
public:

    explicit LeaderElectionClient( const map<string, int> & conf_hosts);
    
    /*
     * @para const Voter & myVoter: my voter
     * @para const string & target_host: ...
     * @return Voter * _voter: target host's voter
     * @comment  why we use Voter pointer? because we want to use NULL identifying connection failure.
     */
    void exchange( Voter * _voter,const Voter & myVoter,const string & target_host );

    /*
     * @para const string & my_host_name: my host name 
     * @para const string & target_host: target host name
     * @return if target is dead or my_host_name is not the largest, false
     * @comment ask target if i can lead.
     */
    bool isLargest( const Voter & voter,  const string & target_host );

    /*
     * oneway function
     * @para const string & message : a string message
     * @para const string & target_host : target host of this message
     * @return connection failure then false, or true
     */
    bool sendMessage( const string & message, const string & target_host );
};

#endif
