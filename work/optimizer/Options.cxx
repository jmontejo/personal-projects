#include "Options.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

Options::Options(TString jo){

  vars.clear();
  noCheck = false;
  cut = weight = "";
  std::set<TString> var_set;
  TString key, value;
  ifstream infile(jo.Data());
  std::string line;
  while(getline(infile,line)){
    std::istringstream ss(line);
    ss  >> key >> value;
    std::cout << key << value <<std::endl;
    if(key=="" || key.Contains("#")) continue;
    else if(key=="var"){
      var_set.insert(value);
    }
    else if(key=="weight"){
      if(weight!=""){
        std::cout << "Duplicated key: weight" <<std::endl;
        exit(1);
      }
      weight = value;
    }
    else if(key=="cut"){
      if(cut!=""){
        std::cout << "Duplicated key: cut" <<std::endl;
        exit(1);
      }
      cut = value;
    }
    else if(key=="noCheck"){
      if(noCheck!=false){
        std::cout << "Duplicated key: noCheck" <<std::endl;
        exit(1);
      }
      noCheck = false;
      if (value=="true" || value=="True")
        noCheck = true;
    }
    else {
      std::cout << "Unknown key: "<< key <<std::endl;
      exit(1);
    }
  }

  for(std::set<TString>::iterator it=var_set.begin(); it!=var_set.end(); it++)
    vars.push_back(*it);

}

std::vector<TString> Options::getVars(){
  return vars;
}
bool Options::getNoCheck(){
  return noCheck;
}
TString Options::getCut(){
  return cut;
}
TString Options::getWeight(){
  return weight;
}

