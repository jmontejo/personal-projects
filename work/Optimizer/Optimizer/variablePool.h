#ifndef variablePool_H
#define variablePool_H

#include <set>
#include <vector>
#include "TString.h"
#include "TTree.h"
#include "TCanvas.h"
#include "Optimizer/Options.h"

class variablePool{

public:
  variablePool(TTree *sigtree, TTree *bkgtree, Options *options, bool skipPlots);
  ~variablePool();
  std::vector<TString> functionVars;
  std::vector<TString> intVars;
  std::vector<TString> doubleVars;
  void AddFunctionVar(TString var);
  void AddVectorVar(TString var);
  void AddDoubleVar(TString var);
  void AddIntVar(TString var);
  void Print();
  TTree* getSigTree();
  TTree* getBkgTree();
  std::vector<TString> GetVarName();
  std::vector<float> GetVarStart();
  std::vector<std::pair<float, int> > GetVarStep();
  std::vector<float> GetVarMin();
  std::vector<float> GetLimMin();
  std::vector<float> GetVarMax();
  std::vector<float> GetLimMax();
  std::vector<float> var_mean;
  std::vector<float> var_min;
  std::vector<float> var_max;
  std::vector<float> lim_min;
  std::vector<float> lim_max;
  std::vector<float> var_start;
  std::vector<std::pair<float, int> > var_step;

private:
  void AddVar(TString var,std::vector<TString> *vec);
  void GetVarsFromTrees();
  void doPlot(TString var,TH1F *hsig, TH1F *hbkg);
  bool emptyMiddleBins(TH1F *h);
  int  getRebinN(TH1F *h);
  void fillVarStart();
  void fillVarStep();

	TFile *dummy;
  TTree *m_sigtree;
  TTree *m_bkgtree;
  TString m_cut, m_weight;
  bool m_noCheck;
  std::vector<TString> m_vars;
  std::set<TString> m_vars_set;
  std::vector<float> m_vars_step;
  TCanvas c1;
  TString plotfolder;
  bool m_skipPlots;

};

#endif
