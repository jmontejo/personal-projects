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
    std::vector<std::pair<TString, TString> > getAlgorithms();
    TString get(TString key);
    void set(TString key, TString value);
    bool getNoCheck();

  private:
    std::vector<TString> vars;
    std::vector<float> steps;
    std::vector<TString> bkgs;
    std::map<TString, TString> options;
    std::vector<std::pair<TString, TString> >algorithms;

};
#endif
