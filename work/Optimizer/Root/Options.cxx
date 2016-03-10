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
  TString key, value, token;
  std::ifstream infile(jo.Data());
  if(!infile.good()){
    std::cout << "Problem reading "<<jo <<std::endl;
    exit(2);
  }
  std::string line;
  std::vector<TString> tokens;
  while(getline(infile,line)){
		tokens.clear();
    std::istringstream iss(line);
    while (iss){ iss>> token; tokens.push_back(token); }
		if(tokens.size()<2 || tokens.at(0).BeginsWith("#")) continue;
    key = tokens.at(0);
		value = tokens.at(1);
    if(key=="" || key.Contains("#")) continue;
    else if(key=="var"){
			if(tokens.size()!=3){
				std::cout << "Badly formed var:step : "<<line <<std::endl;
				exit(2);
			}
  		vars.push_back(value);
  		steps.push_back(tokens.at(2).Atof());
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


}

std::vector<TString> Options::getVars(){
  return vars;
}
std::vector<float> Options::getVarsSteps(){
  return steps;
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
