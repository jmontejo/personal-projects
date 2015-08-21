#ifndef OPTIONS_H
#define OPTIONS_H

#include "TString.h"
#include <vector>

class Options{

  public:
    Options(TString jo);
    std::vector<TString> getVars();
    TString getCut();
    TString getWeight();
    bool getNoCheck();

  private:
    std::vector<TString> vars;
    TString cut;
    TString weight;
    bool noCheck;

};
#endif
