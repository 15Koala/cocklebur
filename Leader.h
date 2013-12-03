/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * an implementation of Leader
 */

#ifndef _LEADER_H
#define _LEADER_H

#include <map>
#include <string>
#include "DataHolder.h"

using namespace std;

class Cocklebur;
class CockleburClient;
class HBMSG;
class Leader: public DataHolder {

private:

    Leader();
    Leader( Leader & );
    Cocklebur * d_cocklebur;
    
    bool d_stat_stop;//< the condition of breaking out following process
    map< string , long > d_lease_mapping;//< HostName => last apply time

    pthread_rwlock_t m_lease_mapping;//< rw lock for d_lease_mapping;
public:

    explicit Leader( Cocklebur * );
    ~Leader();
    void reset();

    void leadering();
    void updateLease( const string & follower );
    void getMessage( HBMSG & );
    void syncData();

};

#endif
