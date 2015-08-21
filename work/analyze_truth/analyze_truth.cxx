#include <stdio.h>
#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include "TopAnalysisBase/TopData.h"
#include "TopD3PDTruthTools/TruthInterface.h"
#include "TopD3PDTruthTools/GeneratorInfo.h"
#include "TInterpreter.h"
#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <map>
#include "Dataset.h"
#include "Output.h"
#include "SampleInfo.h"
#include "TStopwatch.h"

#define DEBUG 1

using namespace std;
void ScanRaw(const TopData *td, const Output *out, bool isSherpa);
int goodJetsSkim(TopData *td);
int convert(int a);

int main(int argn, char *args[]){

  if (argn==1){
    std::cout << "Need an argument: sample" << std::endl;
    return 1;
  }
  TString sample, suffix;
  bool isTest = false;
  bool isSmall = false;
  bool noTaus = false;
  bool onlyHF = false;
  bool onlyPJ = false;
  int events = 1000;
  for(int i=1;i<argn;i++){
    if(i==1) sample = args[i];
    else if(TString(args[i]) == "-s" && i<argn-1) suffix = args[i+1];
    else if(TString(args[i]) == "-e" && i<argn-1) events = atoi(args[i+1]);
    else if(TString(args[i]) == "-t") isTest = true;
    else if(TString(args[i]) == "-T") noTaus = true;
    else if(TString(args[i]) == "-S") isSmall = true;
    else if(TString(args[i]) == "-o") onlyHF  = true;
    else if(TString(args[i]) == "-p") onlyPJ  = true;
  }

  double xsec, ktfac2;
  cout << "TopData" << endl;
  TopData *td = new TopData();
  cout << "Dataset" << endl;
  Dataset *dset = new Dataset(sample,td,&xsec,&ktfac2, isTest ? events : -1);
  cout << "Output" << endl;
  Output *output = new Output(td,&xsec,&ktfac2,sample,suffix,isTest,isSmall,onlyPJ);
  cout << "SampleInfo" << endl;
  SampleInfo *info = new SampleInfo(sample, noTaus);
  cout << "End" << endl;

  TruthInterface *truthInterface = new TruthInterface();
  GeneratorInfo *genInfo = GeneratorInfo::GetGeneratorInfo();
  if(info->type==SampleInfo::Alpgen) genInfo->SetGeneratorInfo(0,GeneratorInfo::herwig,GeneratorInfo::ttQQ);
  else if(info->type==SampleInfo::Sherpa) genInfo->SetGeneratorInfo(0,GeneratorInfo::sherpa,GeneratorInfo::ttbar);
  else if(info->type==SampleInfo::Sherpa2) genInfo->SetGeneratorInfo(0,GeneratorInfo::sherpa2,GeneratorInfo::ttbar);
  else genInfo->SetGeneratorInfo(0,GeneratorInfo::pythia,GeneratorInfo::ttbar);

  int limit = isTest ? min(events,dset->GetEntries()) : dset->GetEntries();
  TStopwatch *timer = new TStopwatch();
  int modulo = 1000;
  std::set<int> eventNumberSet;

  // -- event loop
  int numB=0, numBmatched=0;
  for(int i=0;i<limit;i++){
    if(i%modulo==0){
      Double_t elapsed = timer->RealTime();
      Double_t perevent = elapsed/modulo;
      Double_t eta = (limit-i)*perevent/60.;
      if(i>=modulo && elapsed < 12) //max 2min updates
        modulo = pow(10,trunc(log10(i)));
      std::cout << i << "\tof " << limit << " events "<< std::flush;
      if(i!=0)
        std::cout << "[ "<< (perevent*1000.) <<"\tms/evnt\t| eta: "<<eta<<"\t min ]" << std::flush;
      std::cout << std::endl;
      timer->Start(1);
    }
    dset->GetEntry(i);
    //if(info->type==SampleInfo::Sherpa2 && dset->isLastEntry(i)) continue;
    //ScanRaw(td,NULL,info->type==SampleInfo::Sherpa);
    if(info->type!=SampleInfo::Sherpa2){
      //info->removeEleOverlap(td);
      output->lep_n = info->getLeptonInfo(td,&output->lep_pt,&output->lep_eta, &output->muon_n);
//      if(eventNumberSet.count(td->EventNumber)) continue;
//        eventNumberSet.insert(td->EventNumber);
    }
    info->getTopInfo(td,&output->top_pt,&output->top_eta,&output->ttbar_pt,&output->ttbar_eta,&output->ttbar_TV3);
    output->parton_n = info->GetNpartons(td, output->parton_pdgId, &output->num_ME_b, &output->num_ME_b_fiducial);
    //output->alljet_n = goodJetsSkim(td);

    if(dset->hasHFtype){
      output->passHFtype = truthInterface->classify(td,&output->HFtype, &output->extHFtype,false,dset->isNotDecayed,&output->extHFtype_prompt);
      output->jet_id = dset->d_particle_jet_id;
      output->jet_count = dset->d_particle_jet_count;
      output->jet_trueflav = dset->d_particle_jet_trueflav;
      int b=0, B=0, c=0, C=0;
      int b_prompt=0, B_prompt=0, c_prompt=0, C_prompt=0;
      for(int ii=0; ii< output->jet_id->size(); ii++){
        if(td->jet_AntiKt4Truth_pt->at(ii) < Output::JETPTCUT || fabs(td->jet_AntiKt4Truth_eta->at(ii)) > 2.5) continue;
        if(output->jet_trueflav->at(ii)==5 && output->jet_id->at(ii)<=0){
          if(output->jet_count->at(ii) > 1) B_prompt++;
          else                    b_prompt++;
        }
        if(output->jet_trueflav->at(ii)==4 && output->jet_id->at(ii)==0){
          if(output->jet_count->at(ii) > 1) C_prompt++;
          else                    c_prompt++;
        }
        if(output->jet_trueflav->at(ii)==5 && output->jet_id->at(ii) < 3){
          if(output->jet_count->at(ii) > 1) B++;
          else                    b++;
        }
        if(output->jet_trueflav->at(ii)==4 && (output->jet_id->at(ii)==0 || output->jet_id->at(ii)==-1 || output->jet_id->at(ii)==-2)){
          if(output->jet_count->at(ii) > 1) C++;
          else                    c++;
        }
      }

      output->extHFtype_stored = dset->d_extHFtype;
      output->extHFtype = 1000*b+100*B+10*c+1*C;
      output->extHFtype_prompt = 1000*b_prompt+100*B_prompt+10*c_prompt+1*C_prompt;
      if(b+B > 0)
        output->HFtype = 10*b+B;
      else if(c+C > 0)
        output->HFtype = -(10*c+C);
      if(output->extHFtype_prompt==0 && output->extHFtype!=0){
        output->extHFtype = 0;
        for(int ii=0; ii< output->jet_id->size(); ii++){
          if(td->jet_AntiKt4Truth_pt->at(ii) < Output::JETPTCUT || fabs(td->jet_AntiKt4Truth_eta->at(ii)) > 2.5) continue;
          if(output->jet_id->at(ii)==1 && output->jet_trueflav->at(ii)==5)
            output->extHFtype -= 1000;
          if(output->jet_id->at(ii)==2 && output->jet_trueflav->at(ii)==5)
            output->extHFtype -= 100;
          if(output->jet_id->at(ii)==-1 && output->jet_trueflav->at(ii)==4)
            output->extHFtype -= 10;
          if(output->jet_id->at(ii)==-2 && output->jet_trueflav->at(ii)==4)
            output->extHFtype -= 1;
        }
      }
    }
    else{
      output->passHFtype = truthInterface->classify(td,&output->HFtype, &output->extHFtype,false,dset->isNotDecayed,&output->extHFtype_prompt, Output::JETPTCUT);
      *(output->jet_id) = truthInterface->get_jet_id();
      *(output->jet_count) = truthInterface->get_jet_count();
      *(output->jet_trueflav) = truthInterface->get_jet_trueflav();
      //output->isMPI = info->checkMPI(td);
    }
    int status = truthInterface->computeVariables(td);
    float m_top_pt, m_top_eta, m_ttbar_pt , m_ttbar_eta;
    if(status){
      truthInterface->fillTopReweightInfo(&m_top_pt,&m_top_eta,&m_ttbar_pt,&m_ttbar_eta);
      if(m_ttbar_pt!=output->ttbar_pt)
        std::cout << "FUUUUCK: " << m_ttbar_pt << " " << output->ttbar_pt <<std::endl; 
    }
    else
      std::cout << "WTFFFFFFFFF" <<std::endl;

    if(output->jet_id->size() != td->jet_AntiKt4Truth_n){
      std::cout << output->jet_id->size() << " " << td->jet_AntiKt4Truth_n <<std::endl;
      ScanRaw(td,output,info->type==SampleInfo::Sherpa);
      for(int i=0; i< td->jet_AntiKt4Truth_n; i++)
        std::cout << i << " " << td->jet_AntiKt4Truth_pt->at(i) << " " << td->jet_AntiKt4Truth_eta->at(i) << std::endl;
      for(int i=0; i< output->jet_id->size(); i++)
        std::cout << i << " " << output->jet_id->at(i) << " " << output->jet_trueflav->at(i) << std::endl;
      std::cout << td->EventNumber << " " << output->jet_id->size() << td->jet_AntiKt4Truth_n<< std::endl;
    }

    //if(output->extHFtype==1000 && output->lep_n==1 && fabs(output->q1_pt/1000.-150)<50)
    //  truthInterface->ScanEvent("");
    //if(output->extHFtype==1000 && output->lep_n==1){
    //  int topcount=0;
    //  for(int x=0;x<output->jet_id->size();x++)
    //    if(output->jet_id->at(x)==4) topcount++;
    //  if(topcount!=2)
    //    truthInterface->ScanEvent("");
    //}

    int tmp = output->extHFtype;
    int tmp2 = output->extHFtype_prompt;
    int tmp3 = output->extHFtype_stored;
    if(!onlyHF || output->extHFtype!=0 || (output->extHFtype_stored!=0 && output->extHFtype_stored!=-999)){
      bool scan = output->Fill(sample,td,dset);
      //if (scan) truthInterface->ScanEvent(Form("%d %d %d",tmp, tmp2, tmp3));
      //output->print();
      //if(output->HFtype!=output->HFtypeMatch){
      //  std::cout  << output->HFtype << " " << output->HFtypeMatch << std::endl;
      //ScanRaw(td,output,info->type==SampleInfo::Sherpa);
    }
    }
    std::cout << numB << " " << numBmatched <<std::endl;
    // -- event loop
    output->Write();
    std::cout << "End :)" << std::endl;
    //TCanvas c1("c1");
    //c1.SetLogy(1);

    return 0;
  }


  void ScanRaw(const TopData *td, const Output *out, bool isSherpa){

    cout << Form("hfor, td->mc_channel_number: %d %d",td->top_hfor_type, td->mc_channel_number) << endl;
    for(int p=0;p<td->mc_n && p<2000;p++){
      if(td->mc_pdgId->at(p)==22) continue;
      if(p>10 && td->mc_pdgId->at(p)==21) continue;
      if(abs(td->mc_pdgId->at(p))>100 && !(abs(td->mc_pdgId->at(p))/100==5 || abs(td->mc_pdgId->at(p))/1000==5 ||abs(td->mc_pdgId->at(p))/100==4 ||abs(td->mc_pdgId->at(p))/1000==4)) continue;
      cout << Form("%d %d %d %.2f %.2f %.2f | ",p,td->mc_pdgId->at(p),td->mc_status->at(p),td->mc_pt->at(p),td->mc_eta->at(p),td->mc_phi->at(p)) << flush;
      for(unsigned int pip=0;pip<td->mc_parent_index->at(p).size();pip++)
        cout << td->mc_parent_index->at(p)[pip] << " ";
      if(!isSherpa){
        cout << " | " << flush;
        for(unsigned int pip=0;pip<td->mc_child_index->at(p).size();pip++)
          cout << td->mc_child_index->at(p)[pip] << " (" << td->mc_pdgId->at(td->mc_child_index->at(p)[pip]) <<") " << flush;
      }
      cout << endl;
    }

    cout << "Particle jets: " << td->jet_AntiKt4Truth_n << endl;
    for(int j=0; j< td->jet_AntiKt4Truth_n; j++)
      if(out)
        cout << Form("%d %f %f %f %d %d %d",j,td->jet_AntiKt4Truth_pt->at(j),td->jet_AntiKt4Truth_eta->at(j),td->jet_AntiKt4Truth_phi->at(j),out->jet_id->at(j),out->jet_trueflav->at(j),out->jet_count->at(j)) << endl;
      else
        cout << Form("%d %f %f %f",j,td->jet_AntiKt4Truth_pt->at(j),td->jet_AntiKt4Truth_eta->at(j),td->jet_AntiKt4Truth_phi->at(j)) << endl;

    //getchar();

  }

  int goodJetsSkim(TopData *td){

    int alljets_n = td->jet_AntiKt4Truth_n;
    int index = 0;
    while(index < td->jet_AntiKt4Truth_n){
      if(td->jet_AntiKt4Truth_pt->at(index) < Output::JETPTCUT || fabs(td->jet_AntiKt4Truth_eta->at(index)) > 2.5){
        td->jet_AntiKt4Truth_n--;
        td->jet_AntiKt4Truth_pt ->erase(td->jet_AntiKt4Truth_pt ->begin()+index);
        td->jet_AntiKt4Truth_eta->erase(td->jet_AntiKt4Truth_eta->begin()+index);
        td->jet_AntiKt4Truth_phi->erase(td->jet_AntiKt4Truth_phi->begin()+index);
        td->jet_AntiKt4Truth_E  ->erase(td->jet_AntiKt4Truth_E  ->begin()+index);

      }
      else
        index++;
    }

    return alljets_n;
  }

  int convert(int a){

    int ret = 0;
    int i = abs(a)/10;
    int j = abs(a)%10;
    if(i==1) ret += 10;
    if(i==2) ret += 1;
    if(j==1) ret += 10;
    if(j==2) ret += 1;
    if(a>0)
      return ret;
    return -ret;

  }
