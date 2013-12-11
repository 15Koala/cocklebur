#include "CockNodeTree.h"

CockNodeTree::CockNodeTree( long xid ) {
   d_data_tree = new DataTree; 
   d_data_tree->xid = xid;
}

CockNodeTree::~CockNodeTree() {
    delete d_data_tree;
}

int CockNodeTree::nodeCreate( const string & path, bool isBlocked ) {

}

int CockNodeTree::nodeDelete( const string & path ) {
}

bool CockNodeTree::nodeExist( const string & path ) {
}

string CockNodeTree::getData( const string & path ) {
}

void CockNodeTree::setData( const string & path, const string & data ) {
}

vector < Node > CockNodeTree::getChildren( const string & path ) {
}
DataTree CockNodeTree::getDataTree( const string & path ) {
}
vector < LogEntry > CockNodeTree::getLogEntry( long xid ) {
}

// Backup
void CockNodeTree::snapShot() {
}

void CockNodeTree::logAppend( const LogEntry & log_entry) {
}

