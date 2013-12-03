/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * interface ELECTION 
 */

#ifndef _ELECTION_H
#define _ELECTION_H

class Voter;
class Election {

private:

    virtual void getVoter( Voter & ) = 0;
public:

    Election(){}
    virtual void lookForLeader() = 0;
};

#endif
