#include "Optimizer/Options.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

Options::Options(TString jo){

  vars.clear();
  bkgs.clear();
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
    else if(key=="alg"){
			if(tokens.size()!=3){
				std::cout << "Badly formed Algorithm, alg name options:optx "<< line <<std::endl;
				exit(2);
			}
  		algorithms[value] = tokens.at(2);
    }
    else if(key=="bkg"){
      bkgs.push_back(value);
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

void Options::set(TString key, TString value){
  options[key] = value;
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
std::map<TString, TString> Options::getAlgorithms(){
  return algorithms;
}
TString Options::get(TString key){
  if(options.find(key) == options.end() ){
    std::cout << "Tried to read non-existent key: "<<key <<std::endl;
    exit(1);
  }
  return options[key];
}
