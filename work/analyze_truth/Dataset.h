#ifndef DATASET_H
#define DATASET_H

#include "TopAnalysisBase/TopData.h"
#include "TChain.h"
#include <vector>

using namespace std;

class Dataset{

  public:
    Dataset(TString sample, const TopData *td, double *xsec, double *ktfac2, int maxEntries);
    int GetEntry(int i); //check Int_t GetTreeNumber() const
    int GetEntries(){ return chain->GetEntries(); };
    bool hasHFtype;
    Int_t d_HFtype;
    Int_t d_extHFtype;
    Int_t d_extHFtype_prompt;
    bool d_passHFtype;
    bool isLastEntry(int i);
    bool isNotDecayed;
    vector<int> *d_particle_jet_id;
    vector<int> *d_particle_jet_trueflav;
    vector<int> *d_particle_jet_count;

  private:
    TChain *chain;
    int tree_num;
    vector<double> xsec;
    double *xsec_pointer;
    vector<double> ktfac2;
    double *ktfac2_pointer;
    map<TString,float> xsec_map;
    map<TString,float> kfac_map;
    map<TString,float> ktfac2_map;
    bool isNtupTruth, is7TeV, isNtupTop;
    int d_mcevt_pdf_id1, d_mcevt_pdf_id2;
    UInt_t d_jet_AntiKt4Truth_n;
    Int_t d_mc_channel_number;
    Int_t d_eventNumber;
    TopData *m_td;
    void loadFiles(TString sample, int maxEntries);
    void SetBranchAddresses();
    
};

#endif
