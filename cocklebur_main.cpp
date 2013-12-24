#include <iostream>
#include "utils.h"
#include "Configuration.h"
#include "Cocklebur.h"

//test
#include "utils_time.h"
#include "utils.h"
#include <vector>
#include "gen-cpp/data_holder_types.h"
#include "CLogFile.h"
using namespace std;

#include "CockNodeTree.h"

int main(int argc, char **argv){
   /* 
    if(argc != 2){
	cout<<"usage: ./cocklebur conf_file_path"<<endl;
	cout<<"we will use the default path: conf/cocklebur.xml."<<endl;
	Configuration::init("conf/cocklebur.xml");
    }else{
	Configuration::init(argv[1]);
    }

    Cocklebur cocklebur(Configuration::getConfigMapping());
    cocklebur.printCockleburInfo();
    cocklebur.run();
*/

    CockNodeTree cnt(1);
    cnt.nodeCreate("/root");
    cnt.nodeCreate("/root/c1");
    cnt.nodeCreate("/root/c2");
    cnt.nodeCreate("/root/c2/c21");
    cout<<cnt.toString()<<endl;
    cnt.nodeDelete("/root/c2/c21");
    cout<<cnt.toString()<<endl;

    CLogFile c_log_file("data/0000000000000001.clog");
    vector<LogEntry> log_entries;
    c_log_file.scanLog(0,get_cur_timestamp(),log_entries);
    c_log_file.printLogEntries(log_entries);

    CockNodeTree c2("data/0000000000000001.csnapshot");
    cout<<"-------"<<endl;
    cout<<c2.toString()<<endl;

    return 0;
}
