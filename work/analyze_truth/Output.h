#include "TopAnalysisBase/TopData.h"
#include "TTree.h"
#include "TVector3.h"
#include "TFile.h"
#include "EventShape.h"
#include "Dataset.h"
#include "Reweight.h"


class Output {

  public:
    const static int JETPTCUT = 15000;
    Output(TopData *td, double *xsec, double *ktfac2, TString sample, TString suffix, bool isTest, bool isSmall, bool onlyPJ);
    void clear();

    vector<float> *jet_hffrac;
    vector<int> *jet_trueflav;
    vector<int> *jet_count;
    vector<int> *jet_id;
    vector<int> *parton_pdgId;
    bool isMPI;
    int alljet_n;
    int parton_n;
    int num_ME_b;
    int num_ME_b_fiducial;
    double evweight;
    double evweight1;
    double evweight2;
    double evweight3;
    int mcevt_pdf_id1, mcevt_pdf_id2;
    int lep_n, muon_n, add_jet_n;
    float maxdeltaeta, centrality, aplanarity;
    float lep_pt ;
    float lep_eta;
    float lep_phi;
    float qq_dr, qq_pt, qq_ht, qq_m;
    float q1_eta, q1_pt, q2_pt, var1, var2;
    float top_pt, top_eta, ttX_pt, ttbar_pt, ttbar_eta;
    float rw_toppt, rw_ttbarpt, rw_ttbarpt_toppt, rw_HF, rw_HFnorm, rw_HFshape, rw_HFshapett;
    int HFtype, extHFtype, extHFtype_stored, extHFtype_prompt;
    TVector3 ttbar_TV3;
    bool passHFtype;
    void fillQQ(const TopData *td);
    bool Fill(TString sample, const TopData *td, const Dataset *dset);
    void Write(){ outtree->Write(); };
    void print();

  private:
    TTree *outtree;
    TFile *outfile;
    EventShape *evshape;
    Reweight *m_rw;

};
