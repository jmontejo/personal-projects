#include <iostream>
#include "computeVar.h"
#include "variablePool.h"
#include "TMinuit.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include <cmath>

double (*f)();
void SoverB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void SoverSqrtBSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void getSB(Int_t &npar,  Double_t *par, Double_t *sig_integral, Double_t *bkg_integral );
void setStartingPoint(TMinuit *gMinuit);
void PrintFit();
variablePool *pool;
Double_t sig_integral, bkg_integral, min_value;

bool debug = 1;
TString weight;

int main(int argn, char *args[]){
  
  if(argn < 2){
    std::cout << "Usage: ./optimizer JobOptionFile [-n]" <<std::endl;
  }
  gInterpreter->GenerateDictionary("computeVar","computeVar.cxx");
  TString jofile = args[1];
  Options *options = new Options(jofile);
  weight = options->getWeight();


  TFile *bkgfile = TFile::Open("ttbar_skimmed.root");
  TFile *sigfile = TFile::Open("ttH125_skimmed.root");
  TString treename = "atree";
  //TString treename = "Muon/4jetin25252525_0elecex25topcommonetacommon_1muonex25topcommonetacommon_MET0_MTW_MET0_-1/0btagin0.7892MV1/atree";
  TTree *bkgtree = (TTree *) bkgfile->Get(treename);
  TTree *sigtree = (TTree *) sigfile->Get(treename);

  pool = new variablePool(bkgtree,sigtree,options);
  pool->Print();

  const int N = (pool->functionVars.size() + pool->intVars.size() + pool->doubleVars.size());
  TMinuit *gMinuit = new TMinuit(N);
  if(debug) gMinuit->SetPrintLevel(1);
  else      gMinuit->SetPrintLevel(-1);
  Double_t vstart[N];
  Double_t step[N];


  // Now ready for minimization step
  Double_t arglist[2];
  arglist[0] = 100*N;
  arglist[1] = 1.;

  Int_t ierflg = 0;
//  std::cout << "------- SoverB -------" << std::endl;
//  ierflg = 0;
//  setStartingPoint(gMinuit);
//  gMinuit->SetFCN(SoverB);
//  gMinuit->mnexcm("SEEK", arglist ,2,ierflg);
//  PrintFit();
  std::cout << "------- SoverSqrtB -------" << std::endl;
  ierflg = 0;
  setStartingPoint(gMinuit);
  //gMinuit->SetFCN(SoverB);
  gMinuit->SetFCN(SoverSqrtB);
  gMinuit->mnexcm("SEEK", arglist ,2,ierflg);
  PrintFit();
  arglist[0] = 100*N;
  ierflg = 0;
  // one can try to minimize on top of the best seek
  //setStartingPoint(gMinuit);
  gMinuit->mnexcm("MINIZE", arglist ,2,ierflg);
  PrintFit();
  arglist[0] = 100*N;
//  std::cout << "------- SoverSqrtBSqrtB -------" << std::endl;
//  ierflg = 0;
//  setStartingPoint(gMinuit);
//  gMinuit->SetFCN(SoverSqrtBSqrtB);
//  gMinuit->mnexcm("SEEK", arglist ,2,ierflg);
//  PrintFit();
//  arglist[0] = 50000;
//  gMinuit->mnexcm("MINIMIZE", arglist ,2,ierflg);
//  gMinuit->mnprin(1,gMinuit->fAmin);
//  gMinuit->mnexcm("HESSE", arglist ,2,ierflg);

  delete options;
}

// --- Get integrals
void getSB(Int_t &npar,  Double_t *par, Double_t *sig_integral, Double_t *bkg_integral ){

  TString evcut    = "1";
  int iN = 0;
  TString var;
  for(int i=0;i<pool->functionVars.size(); i++){
    var = pool->functionVars.at(i);
    evcut += Form("*(%s > %g)",var.Data(),par[iN]);
    iN  += 1;
  }
  for(int i=0;i<pool->doubleVars.size(); i++){
    var = pool->doubleVars.at(i);
    evcut += Form("*(%s > %g)",var.Data(),par[iN]);
    iN  += 1;
  }
  evcut += "*"+weight;
  if(debug)
    std::cout << evcut << std::endl;

  TTree *sigtree = pool->getSigTree();
  TTree *bkgtree = pool->getBkgTree();

  TString signame = "hsig";
  signame = signame.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","");
  sigtree->Draw("1 >>"+signame,evcut);
  TH1F *hsig = (TH1F *) gDirectory->Get(signame);
  *sig_integral = hsig->Integral();
  TString bkgname = "hbkg";
  bkgname = bkgname.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","");
  bkgtree->Draw("1 >>"+bkgname,evcut);
  TH1F *hbkg = (TH1F *) gDirectory->Get(bkgname);
  *bkg_integral = hbkg->Integral();

}

// --- Figures of merit
void SoverB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

  getSB(npar,par,&sig_integral,&bkg_integral);
  f = -sig_integral/(bkg_integral+0.00001); 
  min_value = f;
  
}

void SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

  getSB(npar,par,&sig_integral,&bkg_integral);
  f = -sig_integral/sqrt(bkg_integral+0.00001);
  min_value = f;
  if(debug)
    std::cout << "Sig/sqrt(bkg): " << sig_integral<<"/"<<sqrt(bkg_integral)<< " = " << -f << std::endl;
  
}

void SoverSqrtBSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

  getSB(npar,par,&sig_integral,&bkg_integral);
  f = -sig_integral/sqrt(bkg_integral+0.00001+sqrt(bkg_integral));
  min_value = f;
  if(debug)
    std::cout << "Sig/bkg: " << sig_integral<<"/"<<bkg_integral<< " = " << -f << std::endl;
  
}

// --- Set starting values and step sizes for parameters
void setStartingPoint(TMinuit *gMinuit){
  int iN = 0;
  Int_t ierflg = 0;
  TString var;
  for(int i=0;i<pool->functionVars.size(); i++){
    var = pool->functionVars.at(i);
    gMinuit->mnparm(iN  , var, pool->var_mean[var],(pool->var_mean[var]-pool->var_min[var])/1.01, pool->var_min[var],pool->var_max[var],ierflg);
    iN += 1;
  }
  for(int i=0;i<pool->doubleVars.size(); i++){
    var = pool->doubleVars.at(i);
    gMinuit->mnparm(iN  , var, pool->var_mean[var],(pool->var_mean[var]-pool->var_min[var])/1.01, pool->var_min[var],pool->var_max[var],ierflg);
    iN += 1;
  }
}

void PrintFit(){
  gMinuit->mnprin(1,gMinuit->fAmin);
  std::cout << "Sig/bkg: " << sig_integral<<"/"<<bkg_integral<< " -> " << -min_value << std::endl;
}
