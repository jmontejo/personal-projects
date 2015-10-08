#include "Optimizer/Options.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

Options::Options(TString jo){

  vars.clear();
  bkgs.clear();
  noCheck = false;
  std::set<TString> var_set;
  std::set<TString> bkg_set;
  TString key, value;
  std::ifstream infile(jo.Data());
  if(!infile.good()){
    std::cout << "Problem reading "<<jo <<std::endl;
    exit(2);
  }
  std::string line;
  while(getline(infile,line)){
    std::istringstream ss(line);
    ss  >> key >> value;
    if(key=="" || key.Contains("#")) continue;
    else if(key=="var"){
      var_set.insert(value);
    }
    else if(key=="bkg"){
      bkgs.push_back(value);
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
    else{
      if(options.find(key) != options.end() ){
        std::cout << "Duplicated key: "<<key <<std::endl;
        exit(1);
      }
      options[key] = value;
    }
  }

  for(std::set<TString>::iterator it=var_set.begin(); it!=var_set.end(); it++)
    vars.push_back(*it);

}

std::vector<TString> Options::getVars(){
  return vars;
}
std::vector<TString> Options::getBkgs(){
  return bkgs;
}
bool Options::getNoCheck(){
  return noCheck;
}
TString Options::get(TString key){
  if(options.find(key) == options.end() ){
    std::cout << "Tried to read non-existent key: "<<key <<std::endl;
    exit(1);
  }
  return options[key];
}
