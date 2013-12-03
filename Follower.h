/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * an implementation of Election
 */

#ifndef _FOLLOWER_H
#define _FOLLOWER_H

#include "DataHolder.h"

class Cocklebur;
class CockleburClient;
class Follower: public DataHolder{

private:

    Follower();
    Follower( Follower & );
    Cocklebur * d_cocklebur;
    CockleburClient * d_cock_client;
    
    bool d_stat_stop;//< the condition of breaking out following process

public:

    explicit Follower( Cocklebur * );
    ~Follower();

    void following();
    void syncData();
};

#endif
