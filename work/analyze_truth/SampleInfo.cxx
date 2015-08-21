#include "TVector3.h"
#include "SampleInfo.h"
#include <assert.h>
#include <cstdlib>
#include <cmath>
#include <iostream>

SampleInfo::SampleInfo(TString sample, bool noTaus){

  m_noTaus = noTaus;
  if (sample.Contains("Alpgen")){ 
    type = Alpgen;
    ini_status1 = 123;
    ini_status2 = 124;
    top_status  = 155;
  }
  else if(sample.Contains("Zbb")){
    type = Madgraph;
    ini_status1 = 3;
    ini_status2 = 3;
    top_status  = 3;
  }
  else if(sample.Contains("Madgraph")){
    type = Madgraph;
    ini_status1 = 3;
    ini_status2 = 3;
    top_status  = 3;
  }
  else if(sample.Contains("PowhegJimmy") || sample.Contains("PowhegHerwig")){
    type = PowhegHerwig;
    ini_status1 = 123;
    ini_status2 = 124;
    top_status  = 155;
  }
  else if(sample.Contains("PowhegPythia")){
    type = PowhegPythia;
    ini_status1 = 3;
    ini_status2 = 3;
    top_status  = 3;
  }
  else if(sample.Contains("Sherpa2")){
    type = Sherpa2;
    ini_status1 = 3;
    ini_status2 = 3;
    top_status  = 3;
  }
  else if(sample.Contains("Sherpa")){
    type = Sherpa;
    ini_status1 = 3;
    ini_status2 = 3;
    top_status  = 3;
  }
  else if(sample.Contains("aMcAtNlo")){
    type = aMcAtNlo;
    ini_status1 = 11;
    ini_status2 = 11;
    top_status  = 11;
    m_noTaus = true;
  }
  else if(sample.Contains("ttA") || sample.Contains("ttH")){
    type = PowhegPythia; //whatever
    ini_status1 = 3;
    ini_status2 = 3;
    top_status  = 3;
  }
  else{
    std::cout << "SampleInfo unknown: " << sample << std::endl;
    exit(1);
  }

}

int SampleInfo::GetNpartons(const TopData *td, vector<int> *pdgId, int *num_ME_b, int *num_ME_b_fiducial){

    bool top = false;
    int parton_n = 0;
    *num_ME_b = 0;
    *num_ME_b_fiducial = 0;
    for(int i=0; i< td->mc_n; i++){
      if(td->mc_status->at(i)==3){
        if(abs(td->mc_pdgId->at(i))==6) top=true;
        if(top && (abs(td->mc_pdgId->at(i))<=5 || abs(td->mc_pdgId->at(i))==21)){
          parton_n++;
          pdgId->push_back(td->mc_pdgId->at(i));
          if(abs(td->mc_pdgId->at(i))==5){
            *num_ME_b = *num_ME_b + 1;
            if(fabs(td->mc_eta->at(i))<=2.5){
              *num_ME_b_fiducial = *num_ME_b_fiducial + 1;
            }
          }
        }
        if(abs(td->mc_pdgId->at(i))==24) break;
      }
    }

    //if(parton_n>=2){
    //std::cout << parton_n << std::endl;
    //for(int i=0; i< td->mc_n; i++){
    //  if(td->mc_status->at(i)==3){ in3 = true;
    //    std::cout << i << " " << td->mc_pdgId->at(i) << std::endl;
    //  }
    //  else if(td->mc_status->at(i)!=3 && in3)
    //    break;
    //}
    //exit(1);
    //}
    return parton_n;
}

int SampleInfo::removeEleOverlap(TopData *td){

  const float dRcut=0.2;
  float dRmin ;
  TVector3 jet, ele;
  int ijet=-1;
  int removed = 0;

  //Sherpa2 needs different treatment
  if(type==Sherpa2){
    bool in3 = false;
    for(int i=0; i< td->mc_n; i++)
      //std::cout << i << " " << td->mc_pdgId->at(i) << std::endl;
    for(int i=0; i< td->mc_n; i++){
      if(td->mc_status->at(i)==3) in3 = true;
      else if(td->mc_status->at(i)!=3 && in3){
        if(i+7 >= td->mc_n){
          std::cout << "Out of range" << std::endl;
          continue;
        }
        for(int j=0;j<4;j++){
          if(abs(td->mc_pdgId->at(i+4+j))==11){
            ele.SetPtEtaPhi(td->mc_pt->at(i+4+j),td->mc_eta->at(i+4+j),td->mc_phi->at(i+4+j));
            dRmin=999.;
            for(int j=0; j< td->jet_AntiKt4Truth_n; j++){
              jet.SetPtEtaPhi(td->jet_AntiKt4Truth_pt->at(j),td->jet_AntiKt4Truth_eta->at(j),td->jet_AntiKt4Truth_phi->at(j)); 
              if(ele.DeltaR(jet) < dRmin){
                dRmin = ele.DeltaR(jet);
                ijet = j;
              }
            }
            if(dRmin < dRcut){ 
              removeJet(td,ijet);  
              removed++;
              assert(removed<=2);
            }
          }
        }
        break;
      }
    }
  }
  //End Sherpa2
  else{
    for(int i=0; i< td->mc_n && i < 100; i++){

      if(abs(td->mc_pdgId->at(i))==11 && (td->mc_status->at(i)==ini_status1 || td->mc_status->at(i)==ini_status2)){
        ele.SetPtEtaPhi(td->mc_pt->at(i),td->mc_eta->at(i),td->mc_phi->at(i));
        dRmin=999.;
        for(int j=0; j< td->jet_AntiKt4Truth_n; j++){
          jet.SetPtEtaPhi(td->jet_AntiKt4Truth_pt->at(j),td->jet_AntiKt4Truth_eta->at(j),td->jet_AntiKt4Truth_phi->at(j)); 
          if(ele.DeltaR(jet) < dRmin){
            dRmin = ele.DeltaR(jet);
            ijet = j;
          }
        }
        if(dRmin < dRcut){ 
          removeJet(td,ijet);  
          removed++;
          assert(removed<=2);
        }
      }
    }
  }

  return removed;

}

void SampleInfo::removeJet(TopData *td, int index){

  td->jet_AntiKt4Truth_n--;
  td->jet_AntiKt4Truth_pt ->erase(td->jet_AntiKt4Truth_pt ->begin()+index);
  td->jet_AntiKt4Truth_eta->erase(td->jet_AntiKt4Truth_eta->begin()+index);
  td->jet_AntiKt4Truth_phi->erase(td->jet_AntiKt4Truth_phi->begin()+index);
  td->jet_AntiKt4Truth_E  ->erase(td->jet_AntiKt4Truth_E  ->begin()+index);

}

int SampleInfo::getLeptonInfo(const TopData *td, float *pt, float *eta, int *muon_n){

  float pTmax=0.;
  int ilep=-1;
  int lep_n=0;
  *muon_n=0;
  //Sherpa2 needs different treatment
  if(type==Sherpa2){
    bool in3 = false;
    for(int i=0; i< td->mc_n && lep_n!=2; i++){
      if(td->mc_status->at(i)==3) in3 = true;
      else if(td->mc_status->at(i)!=3 && in3){
        for(int j=0;j<4;j++){
          if(abs(td->mc_pdgId->at(i+4+j))==11 || abs(td->mc_pdgId->at(i+4+j))==13 || abs(td->mc_pdgId->at(i+4+j))==15){
            lep_n++;
            if(td->mc_pt->at(i+4+j) > pTmax){
              ilep = i+4+j;
              pTmax = td->mc_pt->at(i+4+j);
            }
          }
        }
        break;
      }
    }
  }
  //End Sherpa2
  else{
    for(int i=0; i< td->mc_n && lep_n!=2; i++){

      if( (abs(td->mc_pdgId->at(i))==11 || abs(td->mc_pdgId->at(i))==13 || (!m_noTaus && abs(td->mc_pdgId->at(i))==15)) && (td->mc_status->at(i)==ini_status1 || td->mc_status->at(i)==ini_status2) ){
        if(lep_n==0)
          lep_n = td->mc_pdgId->at(i);
        else if (lep_n != td->mc_pdgId->at(i)) 
          lep_n=2;
        if(td->mc_pt->at(i) > pTmax){
          ilep = i;
          pTmax = td->mc_pt->at(i);
        }
        if(abs(td->mc_pdgId->at(i))==13) *muon_n += 1;
      }
    }
    if(lep_n!=0 && lep_n!=2) lep_n=1;
  }
  if(lep_n!=0 && ilep>-1){
    *pt  = td->mc_pt->at(ilep);
    *eta = td->mc_eta->at(ilep);
  }
  else{
    *pt  = -999.;
    *eta = -999.;
  }

  return lep_n;

}

int SampleInfo::getTopInfo(const TopData *td, float *top_pt, float *top_eta, float *ttbar_pt, float *ttbar_eta, TVector3 *ttbar_TV3){

  if(type==Sherpa){ //--- Sherpa
    TVector3 vtop, vantitop;
    TVector3 vb, vw, vlep, vnu;
    bool in3 = false;
    int topcount=0;
    int i1=0,i2=0,pdg1=0,pdg2=0;

    for(int i=0; i< td->mc_n; i++){
      if(td->mc_pt->at(i)<1)
        continue;
      if(td->mc_status->at(i)==3){
        if(abs(td->mc_pdgId->at(i))==5 && ((abs(pdg1)>=11 && abs(pdg1) <=16) || (abs(pdg1)>=1 && abs(pdg1) <=4)) && (abs(pdg1+pdg2)==1) && (i2!=0)){
          if(topcount<2){
            assert(td->mc_pt->at(i)>1);
            assert(td->mc_pt->at(i1)>1);
            assert(td->mc_pt->at(i2)>1);
            vb.SetPtEtaPhi(td->mc_pt->at(i),td->mc_eta->at(i),td->mc_phi->at(i));
            vlep.SetPtEtaPhi(td->mc_pt->at(i1),td->mc_eta->at(i1),td->mc_phi->at(i1));
            vnu.SetPtEtaPhi(td->mc_pt->at(i2),td->mc_eta->at(i2),td->mc_phi->at(i2));
            vw = vlep + vnu;
            if(topcount==0){
              gErrorIgnoreLevel = kError;
              vtop = vw+vb;
              *top_pt = vtop.Pt();
              *top_eta = vtop.Eta();
              gErrorIgnoreLevel = kPrint;
            }
            else if(topcount==1){
              vantitop = vw+vb;
              *ttbar_pt = (vtop+vantitop).Pt();
              *ttbar_eta = (vtop+vantitop).Eta();
              *ttbar_TV3 = (vtop+vantitop);
            }
            topcount++;
          }
          else
            std::cout << "ClassifyHF::find_TopB_Wc - Warning: Found more than 2 tops" << std::endl;
        }

        pdg2 = pdg1;
        i2 = i1;
        pdg1 = td->mc_pdgId->at(i);
        i1 = i;
      }
      if( td->mc_status->at(i)==3 && !in3) in3 = true;
      if( td->mc_status->at(i)!=3 && in3) break;
    }

  }//-- end if Sherpa

  else { //-- not Sherpa
    int top1 = -1, top2 = -1;
    for(int i=0;i<td->mc_n && (top1 <0 || top2 <0) ;i++){
      if(td->mc_pdgId->at(i)==6 && td->mc_status->at(i)==top_status)
        top1 = i;
      else if(td->mc_pdgId->at(i)==-6 && td->mc_status->at(i)==top_status)
        top2 = i;
    }
    if(top1<0 || top2 <0)
      return 1;
    *top_pt = td->mc_pt->at(top1);
    *top_eta = td->mc_eta->at(top1);
    TVector3 vtop, vantitop;
    vtop.SetPtEtaPhi(td->mc_pt->at(top1),td->mc_eta->at(top1),td->mc_phi->at(top1));
    vantitop.SetPtEtaPhi(td->mc_pt->at(top2),td->mc_eta->at(top2),td->mc_phi->at(top2));
    *ttbar_pt = (vtop+vantitop).Pt();
    *ttbar_eta = (vtop+vantitop).Eta();
    *ttbar_TV3 = (vtop+vantitop);
  }//-- end if not Sherpa

  return 0;

}

bool SampleInfo::checkMPI(const TopData *td){
  bool isMPI = false;
      //std::cout << "checkMPI "<< std::endl;
  if(type==PowhegPythia){
    if(abs(td->mc_pdgId->at(td->mc_child_index->at(1).at(0)))==5) return false; //b in pdf
    for(int i=1;i<td->mc_child_index->at(1).size() && !isMPI ;i++){
      int par = td->mc_child_index->at(1).at(i);
      isMPI = checkMPI(td, par);
      //std::cout << par << " " << isMPI << std::endl;
      }
    }
  return isMPI;
}

bool SampleInfo::checkMPI(const TopData *td, int index){
  bool isMPI = false;
      //std::cout << index << std::endl;
  if(abs(td->mc_pdgId->at(index))==5) return true;
  for(int i=0;i<td->mc_child_index->at(index).size() && !isMPI ;i++){
      isMPI = checkMPI(td, td->mc_child_index->at(index).at(i));
      //std::cout << index << " " << isMPI << std::endl;
  }
  return isMPI;
}
