#include <iostream>
#include "utils.h"
#include "Configuration.h"
#include "Cocklebur.h"
using namespace std;

int main(int argc, char **argv){
    
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

    return 0;
}
