#include "Reweight.h"
#include "TFile.h"
#include <iostream>

Reweight::Reweight(){

  rw_file = TFile::Open("analyze_truth_outfiles/outhistos2D.root");
  MG_nominal = (TH2F *) rw_file->Get("MadgraphMPI_nominal");
  MG_Matchup = (TH2F *) rw_file->Get("MadgraphOff_Matchup");
  MG_Q2up = (TH2F *) rw_file->Get("MadgraphOff_Q2up");
  MG_Q2down = (TH2F *) rw_file->Get("MadgraphOff_Q2down");
  sys_file = TFile::Open("analyze_truth_outfiles/HFsystematics2DSherpa_p15.root");

  //Nominal
  Nominal_ttb_tt  = (TH2F *) sys_file->Get("rwmap_Nominal_ttb_top_pt_ttbar_pt");
  Nominal_ttB_tt  = (TH2F *) sys_file->Get("rwmap_Nominal_ttB_top_pt_ttbar_pt");
  Nominal_ttbb_tt = (TH2F *) sys_file->Get("rwmap_Nominal_ttbb_top_pt_ttbar_pt");
  Nominal_tt3b_tt = (TH2F *) sys_file->Get("rwmap_Nominal_tt3b_top_pt_ttbar_pt");
  Nominal_ttb_bb  = (TH2F *) sys_file->Get("rwmap_Nominal_ttb_q1_pt_q1_eta");
  Nominal_ttB_bb  = (TH2F *) sys_file->Get("rwmap_Nominal_ttB_q1_pt_q1_eta");
  Nominal_ttbb_bb = (TH2F *) sys_file->Get("rwmap_Nominal_ttbb_qq_pt_qq_dr");
  Nominal_tt3b_bb = (TH2F *) sys_file->Get("rwmap_Nominal_tt3b_qq_pt_qq_dr");
  Nominal_HFcateg[1] = 1.409530;
  Nominal_HFcateg[2] = 3.409967;
  Nominal_HFcateg[3] = 13.378885;
  Nominal_HFcateg[10] = 0.897919;
  Nominal_HFcateg[11] = 1.971165;
  Nominal_HFcateg[12] = 3.962590;
  Nominal_HFcateg[20] = 1.020948;
  Nominal_HFcateg[21] = 2.150611;
  Nominal_HFcateg[22] = 5.985465;
  Nominal_HFcateg[30] = 1.302485;
  Nominal_HFcateg[31] = 5.899145;
  Nominal_HFcateg[40] = 1.202809;
  Nominal_HFcateg[41] = 2.589763;
  Nominal_HFcateg[50] = 1.289720;

  //PowhegHerwig
  PowhegHerwig_ttb_tt  = (TH2F *) sys_file->Get("rwmap_PowhegHerwig_ttb_top_pt_ttbar_pt");
  PowhegHerwig_ttB_tt  = (TH2F *) sys_file->Get("rwmap_PowhegHerwig_ttB_top_pt_ttbar_pt");
  PowhegHerwig_ttbb_tt = (TH2F *) sys_file->Get("rwmap_PowhegHerwig_ttbb_top_pt_ttbar_pt");
  PowhegHerwig_tt3b_tt = (TH2F *) sys_file->Get("rwmap_PowhegHerwig_tt3b_top_pt_ttbar_pt");
  PowhegHerwig_ttb_bb  = (TH2F *) sys_file->Get("rwmap_PowhegHerwig_ttb_q1_pt_q1_eta");
  PowhegHerwig_ttB_bb  = (TH2F *) sys_file->Get("rwmap_PowhegHerwig_ttB_q1_pt_q1_eta");
  PowhegHerwig_ttbb_bb = (TH2F *) sys_file->Get("rwmap_PowhegHerwig_ttbb_qq_pt_qq_dr");
  PowhegHerwig_tt3b_bb = (TH2F *) sys_file->Get("rwmap_PowhegHerwig_tt3b_qq_pt_qq_dr");

  float PH_bb = 4.63257272752;
  float PP_bb = 4.862682853;
  float PH_cc = 8.78200659903;
  float PP_cc = 12.9041948711;
  float ttbar_XS = 252.89;
  PH_light_SF   = (ttbar_XS - (PP_bb+PP_cc))/(ttbar_XS - (PH_bb+PH_cc));
  PH_ttcc_SF = PP_cc/PH_cc;
  PH_ttbb_SF = PP_bb/PH_bb;

  PowhegHerwig_HFcateg[1]  = 0.869638;
  PowhegHerwig_HFcateg[2]  = 1.475383;
  PowhegHerwig_HFcateg[3]  = 2.910867;
  PowhegHerwig_HFcateg[10] = 1.009421;
  PowhegHerwig_HFcateg[11] = 1.932743;
  PowhegHerwig_HFcateg[12] = 5.531228;
  PowhegHerwig_HFcateg[20] = 1.085366;
  PowhegHerwig_HFcateg[21] = 2.327106;
  PowhegHerwig_HFcateg[22] = 13.536410;
  PowhegHerwig_HFcateg[30] = 2.296195;
  PowhegHerwig_HFcateg[31] = 7.779166;
  PowhegHerwig_HFcateg[40] = 2.743063;
  PowhegHerwig_HFcateg[41] = 2.441590;
  PowhegHerwig_HFcateg[50] = 9.042473;

  //Madgraph
  Madgraph_ttb_tt  = (TH2F *) sys_file->Get("rwmap_Madgraph_ttb_top_pt_ttbar_pt");
  Madgraph_ttB_tt  = (TH2F *) sys_file->Get("rwmap_Madgraph_ttB_top_pt_ttbar_pt");
  Madgraph_ttbb_tt = (TH2F *) sys_file->Get("rwmap_Madgraph_ttbb_top_pt_ttbar_pt");
  Madgraph_tt3b_tt = (TH2F *) sys_file->Get("rwmap_Madgraph_tt3b_top_pt_ttbar_pt");
  Madgraph_ttb_bb  = (TH2F *) sys_file->Get("rwmap_Madgraph_ttb_q1_pt_q1_eta");
  Madgraph_ttB_bb  = (TH2F *) sys_file->Get("rwmap_Madgraph_ttB_q1_pt_q1_eta");
  Madgraph_ttbb_bb = (TH2F *) sys_file->Get("rwmap_Madgraph_ttbb_qq_pt_qq_dr");
  Madgraph_tt3b_bb = (TH2F *) sys_file->Get("rwmap_Madgraph_tt3b_qq_pt_qq_dr");

  float MG_bb = 4.4218125233;
  float MG_cc = 11.1851496551;
  MG_light_SF   = (ttbar_XS - (PP_bb+PP_cc))/(ttbar_XS - (MG_bb+MG_cc));
  MG_ttcc_SF = PP_cc/MG_cc;
  MG_ttbb_SF = PP_bb/MG_bb;

  Madgraph_HFcateg[1]  = 1.606335;
  Madgraph_HFcateg[2]  = 3.550287;
  Madgraph_HFcateg[10] = 0.873148;
  Madgraph_HFcateg[11] = 1.953445;
  Madgraph_HFcateg[12] = 6.602378;
  Madgraph_HFcateg[20] = 1.031327;
  Madgraph_HFcateg[21] = 2.242341;
  Madgraph_HFcateg[22] = 1.546224;
  Madgraph_HFcateg[30] = 1.291724;
  Madgraph_HFcateg[31] = 6.215525;
  Madgraph_HFcateg[40] = 1.098458;
  Madgraph_HFcateg[41] = 1.665878;
  Madgraph_HFcateg[50] = 0.413888;
}

void Reweight::reweight(TString sample, float toppt, float ttbarpt, 
    float *rw_toppt, float *rw_ttbarpt, float *rw_ttbarpt_toppt){ 

  if(toppt   >  800e3) toppt = 799e3;
  if(ttbarpt > 1000e3) ttbarpt = 999e3;
  if (sample.Contains("MadgraphOff_Matchup")){
    *rw_ttbarpt_toppt = MG_Matchup->GetBinContent(MG_Matchup->FindBin(toppt/1000.,ttbarpt/1000.));
    return;
  }
  if (sample.Contains("MadgraphOff_Q2up")){
    *rw_ttbarpt_toppt = MG_Q2up->GetBinContent(MG_Q2up->FindBin(toppt/1000.,ttbarpt/1000.));
    return;
  }
  if (sample.Contains("MadgraphOff_Q2down")){
    *rw_ttbarpt_toppt = MG_Q2down->GetBinContent(MG_Q2down->FindBin(toppt/1000.,ttbarpt/1000.));
    return;
  }
  if (sample.Contains("Madgraph")){
    *rw_ttbarpt_toppt = MG_nominal->GetBinContent(MG_nominal->FindBin(toppt/1000.,ttbarpt/1000.));
    return;
  }

  Float_t ttbarPtUpBins[4]= { 40.e3, 170.e3, 340.e3, 1000.e3 };
  Float_t topPtUpBins[7]= { 50.e3, 100.e3, 150.e3, 200.e3, 250.e3, 350.e3, 800.e3 };
  Float_t ttbarPtRatio[4], topPtRatio[7], topPtSeqRatio[7];

  if (sample.Contains("PowhegPythia")){
    ttbarPtRatio[0]  = 1. + 0.0409119;
    ttbarPtRatio[1]  = 1.  -0.0121258;
    ttbarPtRatio[2]  = 1.  -0.188448;
    ttbarPtRatio[3]  = 1.  -0.316183;
    topPtSeqRatio[0] = 1. + 0.0139729;
    topPtSeqRatio[1] = 1. + 0.0128711;
    topPtSeqRatio[2] = 1. + 0.00951743;
    topPtSeqRatio[3] = 1. + 0.00422323;
    topPtSeqRatio[4] = 1.  -0.0352631;
    topPtSeqRatio[5] = 1.  -0.0873852;
    topPtSeqRatio[6] = 1.  -0.120025;
  }
  else if (sample.Contains("PowhegHerwig")){
    ttbarPtRatio[0]  = 1. +0.0790586;
    ttbarPtRatio[1]  = 1.  -0.0438019;
    ttbarPtRatio[2]  = 1.  -0.221803;
    ttbarPtRatio[3]  = 1.  -0.370303;
    topPtSeqRatio[0] = 1. -0.0817317;
    topPtSeqRatio[1] = 1. +0.00234056;
    topPtSeqRatio[2] = 1. +0.0295038;
    topPtSeqRatio[3] = 1. +0.0307822;
    topPtSeqRatio[4] = 1. +0.0178486;
    topPtSeqRatio[5] = 1. +0.0155615;
    topPtSeqRatio[6] = 1. -0.00327289;
  }
  else if (sample.Contains("Madgraph4b")){
    ttbarPtRatio[0] = 0.965579986572;
    ttbarPtRatio[1] = 1.0416008234;
    ttbarPtRatio[2] = 1.03814733028;
    ttbarPtRatio[3] = 1.04566264153;
    topPtRatio[0] =   1.00939404964;
    topPtRatio[1] =   1.01177513599;
    topPtRatio[2] =   1.01707589626;
    topPtRatio[3] =   1.01457488537;
    topPtRatio[4] =   0.952007234097;
    topPtRatio[5] =   0.866840779781;
    topPtRatio[6] =   0.794997990131;
    topPtSeqRatio[0] = 1.01537179947;
    topPtSeqRatio[1] = 1.01482212543;
    topPtSeqRatio[2] = 1.01608777046;
    topPtSeqRatio[3] = 1.01086544991;
    topPtSeqRatio[4] = 0.945108294487;
    topPtSeqRatio[5] = 0.857196450233;
    topPtSeqRatio[6] = 0.782563865185;
  }
  else if (sample.Contains("MadgraphOff")){
    ttbarPtRatio[0] =  0.978779673576;
    ttbarPtRatio[1] =  1.02676892281;
    ttbarPtRatio[2] =  1.01000106335;
    ttbarPtRatio[3] =  1.00781619549;
    topPtRatio[0] =    1.00817060471;
    topPtRatio[1] =    1.01038265228;
    topPtRatio[2] =    1.0167235136;
    topPtRatio[3] =    1.0165116787;
    topPtRatio[4] =    0.956054270267;
    topPtRatio[5] =    0.873585820198;
    topPtRatio[6] =    0.786104381084;
    topPtSeqRatio[0] = 1.0093331337;
    topPtSeqRatio[1] = 1.01089119911;
    topPtSeqRatio[2] = 1.01698601246;
    topPtSeqRatio[3] = 1.01620137691;
    topPtSeqRatio[4] = 0.954322934151;
    topPtSeqRatio[5] = 0.87033188343;
    topPtSeqRatio[6] = 0.780595779419;
  }
  for (unsigned int i=0; i<4; i++) {
    if (ttbarpt < ttbarPtUpBins[i]) {
      *rw_ttbarpt = ttbarPtRatio[i];
      break;
    }
  }
  for (unsigned int i=0; i<7; i++) {
    if (toppt < topPtUpBins[i]) {
      *rw_toppt = topPtRatio[i];
      *rw_ttbarpt_toppt = topPtSeqRatio[i]*(*rw_ttbarpt);
      break;
    }
  }
}

float Reweight::reweightHF(TString sample, int HFtype, float toppt, float ttbarpt, 
    float var1, float var2, float *norm, float *shape, float *shapett){ 
  if(sample.Contains("PowhegPythia"))
    return ttbb_Nominal_rw(HFtype,ttbarpt,toppt,var1,var2,norm,shape, shapett);
  else if(sample.Contains("PowhegHerwig"))
    return ttbb_PowhegHerwig_rw(HFtype,ttbarpt,toppt,var1,var2,norm,shape, shapett);
  else if(sample.Contains("Madgraph"))
    return ttbb_Madgraph_rw(HFtype,ttbarpt,toppt,var1,var2,norm,shape, shapett);
  else{
    std::cout << "Sample not know"<< sample<<std::endl;;
    return 1.;
    }
  
}

//------------------
float Reweight::ttbb_Nominal_rw(int HFtype, float ttbar_pt, float top_pt, float var1, float var2, float *HFnorm, float *HFshape, float *shapett){

  if(!Nominal_HFcateg.count(HFtype))
    return 1.;

  float norm=0, shape=0;
  norm = Nominal_HFcateg[HFtype];

  switch(HFtype){
    case 10:
      shape  = Nominal_ttb_tt->GetBinContent(Nominal_ttb_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= Nominal_ttb_bb->GetBinContent(Nominal_ttb_bb->FindBin(var1,var2));
      break;
    case 1:
      shape  = Nominal_ttB_tt->GetBinContent(Nominal_ttB_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= Nominal_ttB_bb->GetBinContent(Nominal_ttB_bb->FindBin(var1,var2));
      break;
    case 20:
      shape  = Nominal_ttbb_tt->GetBinContent(Nominal_ttbb_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= Nominal_ttbb_bb->GetBinContent(Nominal_ttbb_bb->FindBin(var1,var2));
      break;
    default:
      shape  = Nominal_tt3b_tt->GetBinContent(Nominal_tt3b_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= Nominal_tt3b_bb->GetBinContent(Nominal_tt3b_bb->FindBin(var1,var2));
  }

  if(shape<=0.) shape=1.;
  *HFnorm = norm;
  *HFshape = shape;
  return norm*shape;

}

float Reweight::ttbb_PowhegHerwig_rw(int HFtype, float ttbar_pt, float top_pt, float var1, float var2, float *HFnorm, float *HFshape, float *shapett){

  if(HFtype<0 && HFtype>-90) HFtype=-1;

  if(!PowhegHerwig_HFcateg.count(HFtype))
    return 1.;

  float norm=0, shape=0;
  norm = PowhegHerwig_HFcateg[HFtype];

  if(HFtype<=0)
    return norm;

  switch(HFtype){
    case 10:
      shape  = PowhegHerwig_ttb_tt->GetBinContent(PowhegHerwig_ttb_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= PowhegHerwig_ttb_bb->GetBinContent(PowhegHerwig_ttb_bb->FindBin(var1,var2));
      break;
    case 1:
      shape  = PowhegHerwig_ttB_tt->GetBinContent(PowhegHerwig_ttB_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= PowhegHerwig_ttB_bb->GetBinContent(PowhegHerwig_ttB_bb->FindBin(var1,var2));
      break;
    case 20:
      shape  = PowhegHerwig_ttbb_tt->GetBinContent(PowhegHerwig_ttbb_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= PowhegHerwig_ttbb_bb->GetBinContent(PowhegHerwig_ttbb_bb->FindBin(var1,var2));
      break;
    default:
      shape  = PowhegHerwig_tt3b_tt->GetBinContent(PowhegHerwig_tt3b_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= PowhegHerwig_tt3b_bb->GetBinContent(PowhegHerwig_tt3b_bb->FindBin(var1,var2));
  }

  if(shape<=0.) shape=1.;
  *HFnorm = norm*PH_ttbb_SF;
  *HFshape = shape;
  return norm*shape;

}

float Reweight::ttbb_Madgraph_rw(int HFtype, float ttbar_pt, float top_pt, float var1, float var2, float *HFnorm, float *HFshape, float *shapett){

  if(HFtype<0 && HFtype>-90) HFtype=-1;

  if(!Madgraph_HFcateg.count(HFtype))
    return 1.;

  float norm=0, shape=0;
  norm = Madgraph_HFcateg[HFtype];

  if(HFtype<=0)
    return norm;

  switch(HFtype){
    case 10:
      shape  = Madgraph_ttb_tt->GetBinContent(Madgraph_ttb_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= Madgraph_ttb_bb->GetBinContent(Madgraph_ttb_bb->FindBin(var1,var2));
      break;
    case 1:
      shape  = Madgraph_ttB_tt->GetBinContent(Madgraph_ttB_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= Madgraph_ttB_bb->GetBinContent(Madgraph_ttB_bb->FindBin(var1,var2));
      break;
    case 20:
      shape  = Madgraph_ttbb_tt->GetBinContent(Madgraph_ttbb_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= Madgraph_ttbb_bb->GetBinContent(Madgraph_ttbb_bb->FindBin(var1,var2));
      break;
    default:
      shape  = Madgraph_tt3b_tt->GetBinContent(Madgraph_tt3b_tt->FindBin(top_pt,ttbar_pt));
      *shapett= shape;
      shape *= Madgraph_tt3b_bb->GetBinContent(Madgraph_tt3b_bb->FindBin(var1,var2));
  }

  if(shape<=0.) shape=1.;
  *HFnorm = norm*MG_ttbb_SF;
  *HFshape = shape;

  return norm*shape;

}

