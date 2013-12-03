/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * an implementation of Election
 */

#ifndef _COCK_LEADER_ELECTION_H
#define _COCK_LEADER_ELECTION_H

#include "Election.h"
#include "Cocklebur.h"

class CockleburClient;
class CockLeaderElection: public Election {
private:

    CockLeaderElection(){}

    Cocklebur * d_cocklebur;
    CockleburClient * d_cock_client;// check the host is leader or not
    long d_logical_clock;//< the version of election.
    Voter d_voter; //< a place to hold current voter.
    void getVoter(Voter & _voter);
    long getLogicalClock();
    void logicalClockTick();/// d_logical_clock ++
    void logicalClockTick( long );
    
    vector<string> d_task;//< task host to exchange vote According to the clockwise order.

    // locks 
    pthread_rwlock_t m_logical_clock;
    pthread_rwlock_t m_voter;

public:
    
    ~CockLeaderElection();
    explicit CockLeaderElection(Cocklebur *);
    void lookForLeader();
};

#endif
