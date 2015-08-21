#ifndef SAMPLEINFO_H
#define SAMPLEINFO_H

#include "TopAnalysisBase/TopData.h"
#include "TString.h"

class SampleInfo{

  public:
    typedef enum {Unknown, Alpgen, Madgraph, Sherpa, Sherpa2, PowhegPythia, PowhegHerwig, aMcAtNlo} SampleType;
    SampleInfo (TString sample, bool noTaus);

    int getLeptonInfo(const TopData *td, float *pt, float *eta, int *muon_n);
    int getTopInfo(const TopData *td, float *top_pt, float *top_eta, float *ttbar_pt, float *ttbar_eta, TVector3 *ttbar_TV3);
    int removeEleOverlap(TopData *td);
    int GetNpartons(const TopData *td, vector<int> *pdg, int *num_ME_b, int *num_ME_b_fiducial);
    bool checkMPI(const TopData *td);
    bool checkMPI(const TopData *td, int i);
    SampleType type;

  private:
    void removeJet(TopData *td, int index);
    int ini_status1;
    int ini_status2;
    int top_status;
    bool m_noTaus;

};

#endif
