/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * the defination of the constances.
 */
#ifndef _COCKLEBUR_CONSTANCE_H
#define _COCKLEBUR_CONSTANCE_H

/*
 * Cocklebur nodes have 3 modes. 
 * 1 LOOKING, 2 FOLLWING, 3 LEADERING.
 */
enum CockleburMode { LOOKING = 1, FOLLWING, LEADERING };

class Cocklebur;
class DataHolder;
class Leader;
typedef struct ThreadServerArg {
    int s_port;
    Cocklebur * s_cocklebur;
    int s_test;
} * pArg;

typedef struct CockServerArg {
    int s_port;
    DataHolder * s_data_holder;
} * dArg;

typedef struct LeaderArg {
    int s_port;
    Leader * s_leader;
} * lArg;

#endif
