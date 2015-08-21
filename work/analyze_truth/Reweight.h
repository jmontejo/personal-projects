#include "TString.h"
#include "TH2F.h"
#include "TFile.h"
#include <map>

class Reweight{

public:
  Reweight();
  void reweight(TString sample, float toppt, float ttbarpt, 
                float *rw_toppt, float *rw_ttbarpt, float *rw_ttbarpt_toppt); 
  float reweightHF(TString sample, int HFtype, float toppt, float ttbarpt,
    float var1, float var2, float *norm, float *shape, float *shapett);

private:
  TH2F *MG_nominal;
  TH2F *MG_Matchup;
  TH2F *MG_Q2up;
  TH2F *MG_Q2down;
    std::map<int,float> Nominal_HFcateg;
    TH2F *Nominal_ttb_tt;
    TH2F *Nominal_ttB_tt;
    TH2F *Nominal_ttbb_tt;
    TH2F *Nominal_tt3b_tt;
    TH2F *Nominal_ttb_bb;
    TH2F *Nominal_ttB_bb;
    TH2F *Nominal_ttbb_bb;
    TH2F *Nominal_tt3b_bb;
    std::map<int,float> PowhegHerwig_HFcateg;
    TH2F *PowhegHerwig_ttb_tt;
    TH2F *PowhegHerwig_ttB_tt;
    TH2F *PowhegHerwig_ttbb_tt;
    TH2F *PowhegHerwig_tt3b_tt;
    TH2F *PowhegHerwig_ttb_bb;
    TH2F *PowhegHerwig_ttB_bb;
    TH2F *PowhegHerwig_ttbb_bb;
    TH2F *PowhegHerwig_tt3b_bb;
    std::map<int,float> Madgraph_HFcateg;
    TH2F *Madgraph_ttb_tt;
    TH2F *Madgraph_ttB_tt;
    TH2F *Madgraph_ttbb_tt;
    TH2F *Madgraph_tt3b_tt;
    TH2F *Madgraph_ttb_bb;
    TH2F *Madgraph_ttB_bb;
    TH2F *Madgraph_ttbb_bb;
    TH2F *Madgraph_tt3b_bb;
    float PH_ttcc_SF;
    float PH_ttbb_SF;
    float PH_light_SF;
    float MG_ttcc_SF;
    float MG_ttbb_SF;
    float MG_light_SF;
    TFile *sys_file;
    TFile *rw_file;

  float ttbb_Nominal_rw(int HFtype, float ttbar_pt, float top_pt, float var1, float var2, float *norm, float *shape,float *shapett);
  float ttbb_PowhegHerwig_rw(int HFtype, float ttbar_pt, float top_pt, float var1, float var2, float *norm, float *shape,float *shapett);
  float ttbb_Madgraph_rw(int HFtype, float ttbar_pt, float top_pt, float var1, float var2, float *norm, float *shape,float *shapett);
}; 
