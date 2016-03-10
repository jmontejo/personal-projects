#ifndef OPTIONS_H
#define OPTIONS_H

#include "TString.h"
#include <vector>
#include <map>

class Options{

  public:
    Options(TString jo);
    std::vector<TString> getVars();
    std::vector<float> getVarsSteps();
    std::vector<TString> getBkgs();
    TString get(TString key);
    bool getNoCheck();

  private:
    std::vector<TString> vars;
    std::vector<float> steps;
    std::vector<TString> bkgs;
    std::map<TString, TString> options;
    bool noCheck;

};
#endif
