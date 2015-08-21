#include "Output.h"
#include "TopAnalysisBase/TopData.h"
#include <iostream>

Output::Output(TopData *td, double *xsec, double *ktfac2, TString sample, TString suffix, bool isTest, bool isSmall, bool onlyPJ){

  m_rw = new Reweight();
  evshape = new EventShape();
  TString outname = "analyze_truth_outfiles/"+sample + suffix;
  if(isTest)
    outname += "_test";
  outname += ".root";

  jet_hffrac = new vector<float>();
  jet_trueflav = new vector<int>();
  jet_count = new vector<int>();
  jet_id = new vector<int>();
  parton_pdgId = new vector<int>();

  outfile = TFile::Open(outname,"recreate");
  outtree = new TTree("particle_jets","particle_jets");
  outtree->SetAutoSave();
  outtree->Branch("mc_channel_number",&td->mc_channel_number);
  outtree->Branch("eventNumber",&td->EventNumber);
  if (! onlyPJ){
  outtree->Branch("evweight",&evweight);
  outtree->Branch("xsec",xsec);
  outtree->Branch("ktfac2",ktfac2);
  outtree->Branch("muon_n",&muon_n);
  outtree->Branch("lep_n",&lep_n);
  outtree->Branch("lep_pt",&lep_pt);
  outtree->Branch("lep_eta",&lep_eta);
  outtree->Branch("top_pt",&top_pt);
  outtree->Branch("top_eta",&top_eta);
  outtree->Branch("ttbar_pt",&ttbar_pt);
  outtree->Branch("ttbar_eta",&ttbar_eta);
  outtree->Branch("ttX_pt",&ttX_pt);
  outtree->Branch("rw_ttbarpt_toppt",&rw_ttbarpt_toppt);
  outtree->Branch("rw_HF",&rw_HF);
  outtree->Branch("rw_HFnorm",&rw_HFnorm);
  outtree->Branch("rw_HFshape",&rw_HFshape);
  outtree->Branch("rw_HFshapett",&rw_HFshapett);
  outtree->Branch("qq_dr",&qq_dr);
  outtree->Branch("qq_pt",&qq_pt);
  outtree->Branch("qq_ht",&qq_ht);
  outtree->Branch("qq_m",&qq_m);
  outtree->Branch("q1_eta",&q1_eta);
  outtree->Branch("q1_pt",&q1_pt);
  outtree->Branch("q2_pt",&q2_pt);
  outtree->Branch("HFtype",&HFtype);
  outtree->Branch("extHFtype_prompt",&extHFtype_prompt);
  outtree->Branch("extHFtype",&extHFtype);
  outtree->Branch("extHFtype_stored",&extHFtype_stored);
  outtree->Branch("jet_n",&td->jet_AntiKt4Truth_n);
  outtree->Branch("add_jet_n",&add_jet_n);
  outtree->Branch("num_ME_b",&num_ME_b);
  outtree->Branch("mu",&td->actualIntPerXing);
  }
  outtree->Branch("jet_pt",&td->jet_AntiKt4Truth_pt);
  outtree->Branch("jet_eta",&td->jet_AntiKt4Truth_eta);
  outtree->Branch("jet_phi",&td->jet_AntiKt4Truth_phi);
  outtree->Branch("jet_trueflav",&jet_trueflav);
  outtree->Branch("jet_count",&jet_count);
  outtree->Branch("jet_id",&jet_id);
  //outtree->Branch("jet_AntiKt4Truth_pt",&td->jet_AntiKt4Truth_pt);
  //outtree->Branch("jet_AntiKt4Truth_eta",&td->jet_AntiKt4Truth_eta);
  //outtree->Branch("jet_AntiKt4Truth_phi",&td->jet_AntiKt4Truth_phi);
  //outtree->Branch("particle_jet_trueflav",&jet_trueflav);
  //outtree->Branch("particle_jet_count",&jet_count);
  //outtree->Branch("particle_jet_id",&jet_id);
  if(isSmall){
  outtree->Branch("parton_n",&parton_n);
  outtree->Branch("isMPI",&isMPI);
  outtree->Branch("num_ME_b_fiducial",&num_ME_b_fiducial);
  outtree->Branch("parton_pdgId",&parton_pdgId);
  outtree->Branch("passHFtype",&passHFtype);
  outtree->Branch("rw_toppt",&rw_toppt);
  outtree->Branch("rw_ttbarpt",&rw_ttbarpt);
  outtree->Branch("mcevt_pdf_id1",&mcevt_pdf_id1);
  outtree->Branch("mcevt_pdf_id2",&mcevt_pdf_id2);
  outtree->Branch("hfor",&td->top_hfor_type);
  outtree->Branch("alljet_n",&alljet_n);
  outtree->Branch("jet_hffrac",&jet_hffrac);
  outtree->Branch("maxdeltaeta",&maxdeltaeta);
  outtree->Branch("centrality",&centrality);
  outtree->Branch("aplanarity",&aplanarity);
  outtree->Branch("evweight1",&evweight1);
  outtree->Branch("evweight2",&evweight2);
  outtree->Branch("evweight3",&evweight3);
  }
  clear();

};

void Output::clear(){

  alljet_n = 0;
  parton_n = 0;
  isMPI = false;
  num_ME_b = 0;
  evweight = 0;
  evweight1 = 0;
  evweight2 = 0;
  evweight3 = 0;
  mcevt_pdf_id1 = 0;
  mcevt_pdf_id2 = 0;
  parton_pdgId->clear();
  jet_id->clear();
  jet_hffrac->clear();
  jet_trueflav->clear();
  jet_count->clear();
  lep_n = -1;
  muon_n = -1;
  lep_pt  = -1;
  lep_eta = -1;
  lep_phi = -1;
  aplanarity = centrality = maxdeltaeta = -1;
  qq_dr = qq_ht = qq_pt = qq_m = -1;
  var1 = var2 = q1_eta = q1_pt = q2_pt = -1;
  top_pt = ttX_pt = ttbar_pt = -1;
  top_eta = ttbar_eta = -999;
  HFtype = -999;
  extHFtype_prompt = -999;
  extHFtype = -999;
  extHFtype_stored = -999;
  passHFtype = true;
  rw_toppt = -1;
  rw_ttbarpt = -1;
  rw_ttbarpt_toppt = -1;
  rw_HFnorm = rw_HFshape = rw_HFshapett = rw_HF = -1;

}

void Output::print(){

  std::cout << "Printing Output ----------- " <<std::endl;
//  std::cout << "mc_channel_number" << td->mc_channel_number<< std::endl;
//  std::cout << "eventNumber" << td->EventNumber<< std::endl;
//  std::cout << "hfor" << td->top_hfor_type<< std::endl;
  std::cout << "alljet_n" << alljet_n<< std::endl;
//  std::cout << "jet_n" << td->jet_AntiKt4Truth_n<< std::endl;
//  std::cout << "jet_pt" << td->jet_AntiKt4Truth_pt<< std::endl;
//  std::cout << "jet_eta" << td->jet_AntiKt4Truth_eta<< std::endl;
//  std::cout << "jet_hffrac" << jet_hffrac<< std::endl;
//  std::cout << "jet_trueflav" << jet_trueflav<< std::endl;
//  std::cout << "jet_count" << jet_count<< std::endl;
//  std::cout << "jet_id" << jet_id<< std::endl;
  std::cout << "lep_n" << lep_n<< std::endl;
  std::cout << "lep_pt" << lep_pt<< std::endl;
  std::cout << "lep_eta" << lep_eta<< std::endl;
  std::cout << "top_pt" << top_pt<< std::endl;
  std::cout << "top_eta" << top_eta<< std::endl;
  std::cout << "ttbar_pt" << ttbar_pt<< std::endl;
  std::cout << "ttbar_eta" << ttbar_eta<< std::endl;
  std::cout << "qq_dr" << qq_dr<< std::endl;
  std::cout << "qq_pt" << qq_pt<< std::endl;
  std::cout << "qq_m" << qq_m<< std::endl;
  std::cout << "HFtype" << HFtype<< std::endl;
  std::cout << "passHFtype" << passHFtype<< std::endl;

}

bool Output::Fill(TString sample, const TopData *td, const Dataset *dset){

  //evshape->Reset();
  //for (int i=0;i<td->jet_AntiKt4Truth_n;i++)
  //  evshape->AddParticle(td->jet_AntiKt4Truth_pt->at(i),td->jet_AntiKt4Truth_eta->at(i),td->jet_AntiKt4Truth_phi->at(i),td->jet_AntiKt4Truth_E->at(i));

  //evshape->Evaluate();
  //maxdeltaeta = evshape->GetMaxDeta();
  //centrality = evshape->GetCentrality();
  //aplanarity = evshape->GetAplanarity();  
  if(sample.Contains("Sherpa2")){
  evweight = td->mcevt_weight->at(0).at(0);
  evweight1 = td->mcevt_weight->at(0).at(1);
  evweight2 = td->mcevt_weight->at(0).at(2);
  evweight3 = td->mcevt_weight->at(0).at(3);
  }
  //mcevt_pdf_id1 = td->mcevt_pdf_id1->at(0);
  //mcevt_pdf_id2 = td->mcevt_pdf_id2->at(0);

  if(extHFtype)
    fillQQ(td);
  if(!sample.Contains("Sherpa2")){
  m_rw->reweight(sample, top_pt, ttbar_pt, &rw_toppt, &rw_ttbarpt, &rw_ttbarpt_toppt);
  rw_HF = m_rw->reweightHF(sample, HFtype, top_pt/1000., ttbar_pt/1000., var1, var2, &rw_HFnorm, &rw_HFshape, &rw_HFshapett);
  }
  add_jet_n=0;
  for(int j=0;j<jet_trueflav->size();j++)
    if(jet_trueflav->at(j)!=5) add_jet_n++;
  outtree->Fill();
  //if(td->jet_AntiKt4Truth_n != jet_trueflav->size())
  //  std::cout <<  td->jet_AntiKt4Truth_n << " " <<  td->jet_AntiKt4Truth_pt->size() << " " << jet_trueflav->size()  << " " << jet_id->size() <<std::endl;
  bool scan =(extHFtype==1000 && lep_n==1 && fabs(q1_pt/1000.-150)<50);
  clear();
  return scan;

}

void Output::fillQQ(const TopData *td){
    
    int i1 =-1, i2=-1;
    for(int j=0; j< td->jet_AntiKt4Truth_n && i2<0; j++){
      if(td->jet_AntiKt4Truth_pt->at(j) < JETPTCUT || fabs(td->jet_AntiKt4Truth_eta->at(j)) > 2.5) continue;
      if((extHFtype >= 100 && jet_trueflav->at(j)==5 && jet_id->at(j)<3) ||
         (extHFtype < 100 && jet_trueflav->at(j)==4 && (jet_id->at(j) ==0 || jet_id->at(j) ==-1 || jet_id->at(j) ==-2 )) ){
        if(i1==-1)
          i1 = j;
        else{
          i2 = j;
          break;
        }
      }
    }
    if(i1!=-1){
      q1_pt = td->jet_AntiKt4Truth_pt->at(i1);
      q1_eta= td->jet_AntiKt4Truth_eta->at(i1);
      TVector3 v1;
      v1.SetPtEtaPhi(td->jet_AntiKt4Truth_pt->at(i1),td->jet_AntiKt4Truth_eta->at(i1),td->jet_AntiKt4Truth_phi->at(i1));
      ttX_pt = (ttbar_TV3+v1).Pt();
      var1 = q1_pt/1000.;
      var2 = fabs(q1_eta);
    }
    if(i2!=-1){
      q2_pt = td->jet_AntiKt4Truth_pt->at(i2);
      TLorentzVector v1, v2;
      v1.SetPtEtaPhiE(td->jet_AntiKt4Truth_pt->at(i1), td->jet_AntiKt4Truth_eta->at(i1), td->jet_AntiKt4Truth_phi->at(i1), td->jet_AntiKt4Truth_E->at(i1));
      v2.SetPtEtaPhiE(td->jet_AntiKt4Truth_pt->at(i2), td->jet_AntiKt4Truth_eta->at(i2), td->jet_AntiKt4Truth_phi->at(i2), td->jet_AntiKt4Truth_E->at(i2));
      qq_ht = v1.Pt()+v2.Pt();
      qq_pt = (v1+v2).Pt();
      qq_m = (v1+v2).M();
      qq_dr = v1.DeltaR(v2);
      ttX_pt = (ttbar_TV3+(v1+v2).Vect()).Pt();
      var1 = qq_pt/1000.;
      var2 = qq_dr;
    }
}
