#include "EventShape.h"
#include <iostream>

EventShape::EventShape(){
  fDoMeff      = true;
  fDoTensor    = true;
  fDoFoxWolram = true;
  fDoThrust    = true;
  Reset();
  noJ=true;
}

EventShape::~EventShape(){
}

void EventShape::SetDo(bool doMeff, bool doTensor, bool doFoxWolfram, bool doThrust){
  fDoMeff = doMeff;
  fDoTensor = doTensor;
  fDoFoxWolram = doFoxWolfram;
  fDoThrust = doThrust;
}

void EventShape::Reset(){
  fIsEval = false;
  fPart.clear();
  fMaxDeta = 0.; fSumE = 0., fSumPt = 0.;
  fEvent.SetPtEtaPhiE(0,0,0,0);
  for(int idx=0;idx<3;idx++) fEigenvalues[idx] = 0;
  for(int idx=0;idx<5;idx++) fH[idx] = 0;
  axis.SetXYZ ( 0.0,0.0,0.0 );
}

void EventShape::EvaluateMaxDeta(){

  int size = fPart.size();
  float deta;
  for(int i=0;i<size;i++){
  for(int j=0;j<size;j++){
    deta = fabs(fPart[i].Eta()-fPart[j].Eta());
    if(deta > fMaxDeta)
      fMaxDeta = deta;
  }
  }

}

void EventShape::Evaluate(){
  int size = fPart.size();
  if (fPart.size()==0) return;

  if (size<2) {
    fIsEval = true;
    return;
  }
  
  EvaluateMaxDeta();
  
  fSumE = 0., fSumPt = 0.;
  fSumP = 0., fSumNP = 0.;
  TMatrixDSym MomentumTensor(3);
  double Sxx = 0, Syy = 0, Szz = 0, Sxy = 0, Sxz = 0, Syz = 0, normal = 0;
  for(int idx=0;idx<5;idx++) fH[idx] = 0;
  float x;
  float P[5];
  fEvent.SetPtEtaPhiE(0,0,0,0);
  //
  // thrust stuff
  axis.SetXYZ ( 0.0,0.0,0.0 );
  Double_t diff;
  if(fDoThrust){
    static const int MAX_STEP_THRUST = 20;
    static const double EPSILON = 1e-9;
    axis = fPart[0].Vect();
    axis.SetMag ( 1.0 );  
    int counter = 0;
    do{
      TVector3 axis_tmp = axis;
      axis.SetXYZ ( 0.0,0.0,0.0 );
      for(int i=0;i<size;i++){
        axis += fPart[i].Vect() * TMath::Sign ( 1.0, fPart[i].Vect().Dot ( axis_tmp ) );
      }
      axis.SetMag ( 1.0 );
      diff = axis_tmp.Angle( axis );
      counter++;
    }while ( diff > EPSILON && counter < MAX_STEP_THRUST );
  }  
  // first loop on particles
  for(int i=0;i<size;i++){
    // stuff for cent
    fSumE += fPart[i].E();
    fSumPt += fPart[i].Pt();
    // for Meff
    if(fDoMeff){
      fEvent += fPart[i];
    }
    // for thrust
    if(fDoThrust){
      fSumP  += fPart[i].Vect().Mag();
      fSumNP += TMath::Abs ( fPart[i].Vect().Dot ( axis ) );
    }
    // staff for eigenvalues
    if(fDoTensor){
      Sxx += (fPart[i].Px() * fPart[i].Px());
      Syy += (fPart[i].Py() * fPart[i].Py());
      Szz += (fPart[i].Pz() * fPart[i].Pz());
      Sxy += (fPart[i].Px() * fPart[i].Py());
      Sxz += (fPart[i].Px() * fPart[i].Pz());
      Syz += (fPart[i].Py() * fPart[i].Pz());
      normal += (fPart[i].P() * fPart[i].P());
    }
    // second loop on particles
    if(fDoFoxWolram){
      for(int j=0;j<size;j++){
        x = cos(fPart[i].Angle(fPart[j].Vect()));
        P[0] = 1.;
        P[1] = x;
        P[2] = 0.5*(3*x*x-1);
        P[3] = 0.5*(5*x*x*x-3*x);
        P[4] = 0.125*(35*x*x*x*x-30*x*x+3);
        for(int idx=0;idx<3;idx++) fH[idx] += fabs(fPart[i].P())*fabs(fPart[j].P())*P[idx]; //it was 5
      }
      
      for(int j=i+1;j<size;j++){
        x = cos(fPart[i].Angle(fPart[j].Vect()));
        P[0] = 1.;
        P[1] = x;
	for(int idx=0;idx<2;idx++) fH[idx+3] += fabs(fPart[i].P())*fabs(fPart[j].P())*P[idx]; //it was 5
      }
    }
  }
  if(fDoTensor){
    MomentumTensor(0, 0) = Sxx / normal;
    MomentumTensor(1, 1) = Syy / normal;
    MomentumTensor(2, 2) = Szz / normal;
    MomentumTensor(0, 1) = Sxy / normal;
    MomentumTensor(1, 0) = MomentumTensor(0, 1);
    MomentumTensor(0, 2) = Sxz / normal;
    MomentumTensor(2, 0) = MomentumTensor(0, 2);
    MomentumTensor(1, 2) = Syz / normal;
    MomentumTensor(2, 1) = MomentumTensor(1, 2);
    TMatrixDSymEigen Eigenvalues(MomentumTensor); //Find the Eigen values
    TVectorD EigenvaluesVec = Eigenvalues.GetEigenValues(); //Read the Eigen values ( they are sorted )
    for(int idx=0;idx<3;idx++) fEigenvalues[idx] = EigenvaluesVec(idx);
  }

  fIsEval = true;
}


// SET

void EventShape::SetParticles(vector<TLorentzVector> particles){
  fPart = particles;
}

void EventShape::AddParticle(TLorentzVector part){
  fPart.push_back(part);
}

void EventShape::AddParticle(float pT, float eta, float phi, float E){
  TLorentzVector p;
  p.SetPtEtaPhiE(pT,eta,phi,E);
  AddParticle(p);
}


// GET

float EventShape::GetHT(){
  return fSumPt;
}

float EventShape::GetEffectiveMass(){
  return fEvent.M();
}

float EventShape::GetCentrality(){
  if (fSumE==0) return 0;
  return fSumPt/fSumE;
}

float EventShape::GetAplanarity(){
  return 1.5 * fEigenvalues[2];
}

float EventShape::GetSphericity(){
  return 1.5 * (fEigenvalues[1] + fEigenvalues[2]);
}

float EventShape::GetFoxWolframMoment(int order){
  if (order>4) return 999;
  if ( fSumE*fSumE==0 ) return 0;
  return fH[order] / (fSumE*fSumE);
}

float EventShape::GetMaxDeta(){
  return fMaxDeta;
}

float EventShape::GetPlanarity(){
  return fEigenvalues[1] - fEigenvalues[2];
}

float EventShape::GetVariable_C(){
  return 3.*( fEigenvalues[0]*fEigenvalues[1]+ fEigenvalues[0]* fEigenvalues[2] + fEigenvalues[1]*fEigenvalues[2]);
}

float EventShape::GetVariable_D(){
  return 27.* (fEigenvalues[0]*fEigenvalues[1]* fEigenvalues[2]);
}

float EventShape::GetCircularity(){
  if( (fEigenvalues[0] + fEigenvalues[1]) != 0 )
    return   2. * fEigenvalues[1] / (fEigenvalues[0] + fEigenvalues[1]); 
  else return 0;
}

float EventShape::GetPlanarFlow(){
  if ( ((fEigenvalues[0]+fEigenvalues[1])*(fEigenvalues[0]+fEigenvalues[1]))==0 )
    return 0;
 return   (4*fEigenvalues[0]*fEigenvalues[1])/((fEigenvalues[0]+fEigenvalues[1])*(fEigenvalues[0]+fEigenvalues[1]));
}

float EventShape::GetThrust(){
  if ( fSumP==0 ) return 0;
  return fSumNP/fSumP;  
}

TVector3 EventShape::GetThrustAxis(){
  return axis;
}
