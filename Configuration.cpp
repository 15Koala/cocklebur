#include "Configuration.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
using namespace rapidxml;

map<string,string> Configuration::d_configMap;// initial static member d_configMap
void Configuration::init(const string & path){
    file<> fdoc(path.c_str());
    xml_document<> doc;
    doc.parse<0>(fdoc.data());

    xml_node<>* root = doc.first_node("configuration");
    xml_node<>* node = root->first_node("property");
    xml_node<>* child1;
    xml_node<>* child2;

    string s1,s2;

    for( ; node; node=node->next_sibling())
    {
	child1 = node->first_node("name");
	child2 = node->first_node("value");

	s1 = child1->value();
	s2 = child2->value();
	trim(s1,' ');
	trim(s2,' ');
	d_configMap[s1] = s2;
    }   

}
map<string,string> & Configuration::getConfigMapping(){
    return d_configMap;
}
void Configuration::trim(string & s, char ch){
    int iLen = (int)s.length();
    if(iLen == 0) return;

    int i = 0;
    while(i < iLen)
    {
	if(s[i] == ch)
	    i++;
	else
	    break;
    }   
    int j = iLen - 1;
    while(j > i)
    {       
	if(s[j] == ch)
	    j--;
	else
	    break;
    }

    string sTemp(s, i, j-i+1);
    s = sTemp;
}
string Configuration::get(const string & key){
    map<string,string>::iterator it = d_configMap.find(key);
    if(it != d_configMap.end())
	return d_configMap[key];
    else
	return "";
}
void Configuration::set(const string & key, const string & value){
    d_configMap[key] = value;
}
