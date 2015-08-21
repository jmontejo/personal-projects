#ifndef EventShape_h
#define EventShape_h

#include <vector>
#include <cmath>
#include "TObject.h"
#include "TMatrixDSym.h"
#include "TLorentzVector.h"
#include "TMatrixDSymEigen.h"

using namespace std;

class EventShape : public TObject{
  public:
    EventShape();
    virtual ~EventShape();
    
    void SetDo(bool doMeff, bool doTensor, bool doFoxWolfram, bool doThrust);
    
    void SetParticles(vector<TLorentzVector> particles);
    void AddParticle(float pT, float eta, float phi, float E);
    void AddParticle(TLorentzVector part);
    
    void Reset();
    void Evaluate();
    void EvaluateMaxDeta();
    
    float GetHT();
    float GetEffectiveMass();
    float GetCentrality();
    float GetAplanarity();
    float GetSphericity();
    float GetFoxWolframMoment(int order);
    float GetMaxDeta();

    float GetPlanarity();
    float GetVariable_C();
    float GetVariable_D();
    float GetCircularity();
    float GetPlanarFlow();
    float GetThrust();
    TVector3 GetThrustAxis();

  private:
    bool fDoMeff;
    bool fDoTensor;
    bool fDoFoxWolram;
    bool fDoThrust;
    
    vector<TLorentzVector> fPart;
    float fSumE;
    float fSumPt;
    float fSumP;
    float fSumNP;
    float fEigenvalues[3];
    TLorentzVector fEvent;
    float fH[5];
    float fMaxDeta;

    bool fIsEval;   
    
    bool noJ;

    TVector3 axis;
};

#endif

