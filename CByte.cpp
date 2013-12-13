#include "CByte.h"
#include <cmath>

string CByte::toBytes( int x ) {
    std::size_t size = sizeof(x);
    std::size_t cnt = size;
    string _return(size,0);
    while( cnt-- ) {
	unsigned char byte = (unsigned char)((x>>((size-1-cnt)*8)) & 0xff);
	_return[ cnt ] = byte;
    }
    return _return;
}

string CByte::toBytes( unsigned int x ) {
    std::size_t size = sizeof(x);
    std::size_t cnt = size;
    string _return(size,0);
    while( cnt-- ) {
	unsigned char byte = (unsigned char)((x>>((size-1-cnt)*8)) & 0xff);
	_return[ cnt ] = byte;
    }
    return _return;
}

string CByte::toBytes( long x ) {
    std::size_t size = sizeof(x);
    std::size_t cnt = size;
    string _return(size,0);
    while( cnt-- ) {
	unsigned char byte = (unsigned char)((x>>((size-1-cnt)*8)) & 0xff);
	_return[ cnt ] = byte;
    }
    return _return;
}

string CByte::toBytes( unsigned long x ) {
    std::size_t size = sizeof(x);
    std::size_t cnt = size;
    string _return(size,0);
    while( cnt-- ) {
	unsigned char byte = (unsigned char)((x>>((size-1-cnt)*8)) & 0xff);
	_return[ cnt ] = byte;
    }
    return _return;
}

string CByte::toBytes( double x ) {
    std::size_t size = sizeof(x);
    string _return(size,0);
    char * start = reinterpret_cast<char *>(&x);
    for(std::size_t i = 0; i<size; i++ ) {
	_return[i] = *(start + i);
    }
    return _return;
}

string CByte::toBytes( float x ) {
    std::size_t size = sizeof(x);
    std::size_t cnt = size;
    string _return(size,0);

    return _return;
}

string CByte::toHexString( const string & x ) {
    std::size_t size = x.size() * 2 + 2;
    string _return( size, 0 );
    _return[0] = '0';
    _return[1] = 'x';
    for( int i = 0; i < x.size(); i++ ) {
	int low = static_cast< unsigned char >( x[x.size()-i-1] ) % 16;
	int high = static_cast< unsigned char >( x[x.size()-i-1] ) / 16;
	_return[size-2*i-1] = ( low > 9 )? low + 87 : low + 48 ;
	_return[size-2*i-2] = ( high > 9 )? high + 87 : high + 48 ;
    }
    return _return;
}

int CByte::getInt( const string & x ) {
    return static_cast<int>(CByte::getUInt( x ));
}

unsigned int CByte::getUInt( const string & x ) {
    unsigned int _return = 0;
    for( std::size_t i = 0; i < x.size(); ++i ) {
	_return += static_cast<unsigned char>( x[x.size()-1-i]) *static_cast<unsigned int>(pow( 256, i ));
    }
    return _return;
}

long CByte::getLong( const string & x ) {
    return static_cast<long>(CByte::getULong( x ));
}

unsigned long CByte::getULong( const string & x ) {
    unsigned int _return = 0;
    for( std::size_t i = 0; i < x.size(); ++i ) {
	_return += static_cast<unsigned char>(x[x.size()-1-i])*static_cast<unsigned long>(pow( 256, i ));
    }
    return _return;
}

double CByte::getDouble( const string & x ) {
    char * p = const_cast<char*>(x.c_str());
    double * _return = 0;
    _return = reinterpret_cast<double*>(p);
    return *_return;
}

float CByte::getFloat( const string & x ) {
    char * p = const_cast<char*>(x.c_str());
    float * _return = 0;
    _return = reinterpret_cast<float*>(p);
    return *_return;
}
