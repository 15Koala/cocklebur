/**
 * Copyright 2013 koala.
 * author koala
 * qq: 515539648
 * CByte.h
 */

#ifndef _C_BYTE_H
#define _C_BYTE_H

#include <string>
#include <iostream>

using namespace std;

class CByte {

public:

    // string is considered as char array. 
    // it is byte array.
    
    static string toBytes( int );
    static string toBytes( unsigned int );
    static string toBytes( long );
    static string toBytes( unsigned long );
    static string toBytes( double );
    static string toBytes( float );

    static string toHexString( const string & x );
    static string toHexString( const char * x, unsigned int len);

    static int getInt( const string & x );
    static unsigned int getUInt( const string & x );
    static long getLong( const string & x );
    static unsigned long getULong( const string & x );
    static double getDouble( const string & x );
    static float getFloat( const string & x );

    static int getInt( const char * x, size_t len );
    static unsigned int getUInt( const char * x, size_t len );
    static long getLong( const char * x, size_t len );
    static unsigned long getULong( const char * x, size_t len );
    static double getDouble( const char * x, size_t len );
    static float getFloat( const char * x, size_t len );

};

#endif

