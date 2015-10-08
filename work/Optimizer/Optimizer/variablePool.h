#ifndef variablePool_H
#define variablePool_H

#include <map>
#include <vector>
#include "TString.h"
#include "TTree.h"
#include "TCanvas.h"
#include "Optimizer/Options.h"

class variablePool{

public:
  variablePool(TTree *sigtree, TTree *bkgtree, Options *options, bool skipPlots);
  std::vector<TString> functionVars;
  std::vector<TString> intVars;
  std::vector<TString> doubleVars;
  void AddFunctionVar(TString var);
  void AddVectorVar(TString var);
  void AddDoubleVar(TString var);
  void AddIntVar(TString var);
  void Print();
  std::map<TString, double> var_mean;
  std::map<TString, double> var_min;
  std::map<TString, double> var_max;
  std::map<TString, double> lim_min;
  std::map<TString, double> lim_max;
  std::map<TString, double> var_start;
  TTree* getSigTree();
  TTree* getBkgTree();
  std::vector<float> GetVarStart();
  std::map<TString, double> GetVarMin();
  std::map<TString, double> GetLimMin();

private:
  void AddVar(TString var,std::vector<TString> *vec);
  void GetVarsFromTrees();
  void doPlot(TString var,TH1F *hsig, TH1F *hbkg);
  bool emptyMiddleBins(TH1F *h);
  int  getRebinN(TH1F *h);
  void startVar();

  TTree *m_sigtree;
  TTree *m_bkgtree;
  TString m_cut, m_weight;
  bool m_noCheck;
  std::vector<TString> m_vars;
  TCanvas c1;
  TString plotfolder;
  bool m_skipPlots;

};

#endif
