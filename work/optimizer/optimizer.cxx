#include <iostream>
#include "computeVar.h"
#include "variablePool.h"
#include "TMinuit.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TInterpreter.h"
#include "GeneticAlgorithm.h"
#include <cmath>

double (*f)();
void SoverB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void getSB(Int_t &npar,  Double_t *par, Double_t *sig_integral, Double_t *bkg_integral );
void setStartingPoint(TMinuit *gMinuit);
void PrintBestFit();
variablePool *pool;
Double_t sig_integral, bkg_integral, min_sig, min_bkg, min_value=99999;
std::map<float,std::vector<float> > seekPool;
TGraph graph;
int igraph=0;


bool debug = 0;
TString weight;

int main(int argn, char *args[]){
gROOT->SetBatch(1);
  
  if(argn < 2){
    std::cout << "Usage: ./optimizer JobOptionFile [-n]" <<std::endl;
  }
  gInterpreter->GenerateDictionary("computeVar","computeVar.cxx");
  TString jofile = args[1];
  Options *options = new Options(jofile);
  weight = options->get("weight");


  TFile *bkgfile = TFile::Open(options->get("bkg"));
  TFile *sigfile = TFile::Open(options->get("signal"));
  TString treename = options->get("tree");
  TTree *bkgtree = (TTree *) bkgfile->Get(treename);
  TTree *sigtree = (TTree *) sigfile->Get(treename);

  pool = new variablePool(sigtree,bkgtree,options);
  pool->Print();

  const int N = (pool->functionVars.size() + pool->intVars.size() + pool->doubleVars.size());
  TMinuit *gMinuit = new TMinuit(N);
  if(debug) gMinuit->SetPrintLevel(1);
  else      gMinuit->SetPrintLevel(-1);
  Double_t vstart[N];
  Double_t step[N];


  // Now ready for minimization step
  Double_t arglist[2];
  arglist[0] = 1000*N; //100*N
  arglist[1] = 1.;

  Int_t ierflg = 0;
//  std::cout << "------- SoverB -------" << std::endl;
//  ierflg = 0;
//  setStartingPoint(gMinuit);
//  gMinuit->SetFCN(SoverB);
//  gMinuit->mnexcm("SEEK", arglist ,2,ierflg);
//  PrintBestFit();
  std::cout << "------- SoverSqrtB -------" << std::endl;
  ierflg = 0;
  setStartingPoint(gMinuit);
  //gMinuit->SetFCN(SoverB);
  gMinuit->SetFCN(SoverSqrtB);
  gMinuit->mnexcm("SEEK", arglist ,2,ierflg);
  PrintBestFit();

  //------------ Genetic
  GeneticAlgorithm ga(20);
  ga.SetFCN(N,SoverSqrtB);
  std::vector<float> start = pool->GetVarStart();
  //ga.SetVarStart(start);
  ga.SetInitPool(seekPool);
  ga.Minimize(100);
  std::vector<float> min = pool->GetVarMin();
  ga.Analyze(min);

  // one can try to minimize on top of the best seek
  //arglist[0] = 100*N;
  //ierflg = 0;
  //setStartingPoint(gMinuit);
  //gMinuit->mnexcm("MINIZE", arglist ,2,ierflg);
  //PrintBestFit();
  //
  TCanvas can("can");
  graph.Draw();
  can.SaveAs("graph.png");

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
  //if (f<min_value){
  //min_value = f;
  //min_sig = sig_integral;
  //min_bkg = bkg_integral;
  //min_pars = std::vector<double>(par, par + sizeof(Double_t)*npar);
  //} 
}

void SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

  getSB(npar,par,&sig_integral,&bkg_integral);
  f = -sig_integral/sqrt(bkg_integral+1);
  if (f<min_value){
  min_value = f;
  min_sig = sig_integral;
  min_bkg = bkg_integral;
  std::vector<double> min_pars(par, par + sizeof(Double_t)*npar);
  seekPool[min_value] = std::vector<float>(min_pars.begin(), min_pars.end());
    std::cout << "Sig/sqrt(bkg): " << sig_integral<<"/"<<sqrt(bkg_integral)<< " = " << -f << std::endl;
  } 
  if(debug)
    std::cout << "Sig/sqrt(bkg): " << sig_integral<<"/"<<sqrt(bkg_integral)<< " = " << -f << std::endl;
  graph.SetPoint(igraph,igraph,-f);
  igraph++;
  
}

// --- Set starting values and step sizes for parameters
void setStartingPoint(TMinuit *gMinuit){
  int iN = 0;
  Int_t ierflg = 0;
  TString var;
  for(int i=0;i<pool->functionVars.size(); i++){
    var = pool->functionVars.at(i);
    gMinuit->mnparm(iN  , var, (pool->var_mean[var]+2*pool->var_min[var])/3.,(pool->var_mean[var]-pool->var_min[var])/3.01, pool->var_min[var],pool->var_max[var],ierflg);
    iN += 1;
  }
  for(int i=0;i<pool->doubleVars.size(); i++){
    var = pool->doubleVars.at(i);
    gMinuit->mnparm(iN  , var, (pool->var_mean[var]+2*pool->var_min[var])/3.,(pool->var_mean[var]-pool->var_min[var])/3.01, pool->var_min[var],pool->var_max[var],ierflg);
    iN += 1;
  }
}

void PrintBestFit(){
  gMinuit->mnprin(1,gMinuit->fAmin);
  std::cout << "Sig/bkg: " << min_sig<<"/"<<min_bkg<< " -> " << -min_value << std::endl;
}
