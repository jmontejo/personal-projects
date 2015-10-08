#include <iostream>
#include "Optimizer/computeVar.h"
#include "Optimizer/variablePool.h"
#include "Optimizer/GeneticAlgorithm.h"
#include "TMinuit.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TInterpreter.h"
#include <cmath>
#include <SampleHandler/SampleLocal.h>
#include <SampleHandler/SampleHist.h>
#include "xAODRootAccess/Init.h"

#define SEEK_N 10
#define GA_POOL_SIZE 10
#define GA_MINIM_COUNT 10
#define SEEK_N_TEST 3
#define GA_POOL_SIZE_TEST 2
#define GA_MINIM_COUNT_TEST 2

const char* APP_NAME = "Optimizer";

double (*f)();
void SoverB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void getSB(Int_t &npar,  Double_t *par, Double_t *sig_integral, Double_t *bkg_integral );
void setStartingPoint(TMinuit *gMinuit);
void PrintBestFit(TMinuit *gMinuit);
variablePool *pool;
Double_t sig_integral, bkg_integral, min_sig, min_bkg, min_value=99999;
std::map<float,std::vector<float> > seekPool;
TGraph graph;
int igraph=0;


bool debug = 0;
TString weight;
float bkgweight=1., sigweight=1., lumi=1., bkgsyst=0.;

int main(int argn, char *args[]){
gROOT->SetBatch(1);
  
  if(argn < 2){
    std::cout << "Usage: ./optimizer JobOptionFile [-t]" <<std::endl;
  }
  bool testrun = false;
  int testscale =1;
  if(argn >= 3 && TString(args[2])=="-t")
    testrun = true;
  if(testrun && argn==4)
    testscale=TString(args[3]).Atoi();
    
  gInterpreter->GenerateDictionary("computeVar","computeVar.cxx");
  TString jofile = args[1];
  Options *options = new Options(jofile);
  weight = options->get("weight");
  bkgsyst = options->get("syst").Atof()/100.;
  lumi = options->get("lumi").Atof();


  TString treename = options->get("tree");
  TFile *bkgfile = TFile::Open(options->get("bkg"));
  TFile *sigfile = TFile::Open(options->get("signal"));
  TTree *bkgtree;
  TTree *sigtree;
  if(treename=="sample"){
    xAOD::Init(APP_NAME);
    SH::SampleLocal *sample = (SH::SampleLocal *) bkgfile->Get(treename);
    bkgtree = (TTree *) sample->makeTChain();
    float xs = sample->getMetaDouble("xs");
    TFile *bkgfile_hist = TFile::Open(options->get("bkg").ReplaceAll("output-ntuple","hist"));
    SH::SampleHist *s_hist = (SH::SampleHist *) bkgfile_hist->Get(treename);
    TH1F *cutflow = (TH1F *) s_hist->readHist("DerivationStat_Weights");
    int n_tot = cutflow->GetBinContent(1);
    if(n_tot < 1){
      cutflow = (TH1F *) s_hist->readHist("passedWeights_el_Preselection");
      n_tot = cutflow->GetBinContent(1);
    }
    bkgweight = xs/n_tot;

    sample = (SH::SampleLocal *) sigfile->Get(treename);
    sigtree = (TTree *) sample->makeTChain();
    xs = sample->getMetaDouble("xs");
    TFile *sigfile_hist = TFile::Open(options->get("signal").ReplaceAll("output-ntuple","hist"));
    s_hist = (SH::SampleHist *) sigfile_hist->Get(treename);
    cutflow = (TH1F *) s_hist->readHist("DerivationStat_Weights");
    n_tot = cutflow->GetBinContent(1);
    if(n_tot < 1){
      cutflow = (TH1F *) s_hist->readHist("passedWeights_el_Preselection");
      n_tot = cutflow->GetBinContent(1);
    }
    sigweight = xs/n_tot;

  }
  else{
    bkgtree = (TTree *) bkgfile->Get(treename);
    sigtree = (TTree *) sigfile->Get(treename);
  }

  pool = new variablePool(sigtree,bkgtree,options,testrun);
  pool->Print();

  const int N = (pool->functionVars.size() + pool->intVars.size() + pool->doubleVars.size());
  TMinuit *gMinuit = new TMinuit(N);
  if(debug) gMinuit->SetPrintLevel(1);
  else      gMinuit->SetPrintLevel(-1);
  //Double_t vstart[N];
  //Double_t step[N];


  // Now ready for minimization step
  Double_t arglist[2];
  if(testrun)
    arglist[0] = SEEK_N_TEST*testscale;
  else
  arglist[0] = SEEK_N*N; //100*N
  arglist[1] = 1.;

  Int_t ierflg = 0;
  std::cout << "------- SoverSqrtB -------" << std::endl;
  ierflg = 0;
  setStartingPoint(gMinuit);
  //gMinuit->SetFCN(SoverB);
  gMinuit->SetFCN(SoverSqrtB);
  gMinuit->mnexcm("SEEK", arglist ,2,ierflg);
  PrintBestFit(gMinuit);

  //------------ Genetic
  GeneticAlgorithm ga(GA_POOL_SIZE);
  if (testrun)
    ga.SetMaxPool(GA_POOL_SIZE_TEST*testscale);
  ga.SetFCN(N,SoverSqrtB);
  std::vector<float> start = pool->GetVarStart();
  //ga.SetVarStart(start);
  ga.SetInitPool(seekPool);
  if(testrun)
    ga.Minimize(GA_MINIM_COUNT_TEST*testscale);
  else
    ga.Minimize(GA_MINIM_COUNT);
  std::map<TString, double> lim_min= pool->GetLimMin();
  ga.Analyze(lim_min);

  // one can try to minimize on top of the best seek
  //arglist[0] = 100*N;
  //ierflg = 0;
  //setStartingPoint(gMinuit);
  //gMinuit->mnexcm("MINIZE", arglist ,2,ierflg);
  //
  TCanvas can("can");
  graph.Draw();
  can.SaveAs("graph.png");

  return 0;
}

// --- Get integrals
void getSB(Int_t &npar,  Double_t *par, Double_t *sig_integral, Double_t *bkg_integral ){

  TString evcut    = "1";
  int iN = 0;
  TString var;
  for(unsigned int i=0;i<pool->functionVars.size(); i++){
    var = pool->functionVars.at(i);
    evcut += Form("*(%s > %g)",var.Data(),par[iN]);
    iN  += 1;
  }
  for(unsigned int i=0;i<pool->doubleVars.size(); i++){
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
  *sig_integral = lumi*sigweight*hsig->Integral();
  TString bkgname = "hbkg";
  bkgname = bkgname.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","");
  bkgtree->Draw("1 >>"+bkgname,evcut);
  TH1F *hbkg = (TH1F *) gDirectory->Get(bkgname);
  *bkg_integral = lumi*bkgweight*hbkg->Integral();

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
  f = -sig_integral/sqrt(bkg_integral+pow(bkg_integral*bkgsyst,2)+1);
  if (f<min_value){
  min_value = f;
  min_sig = sig_integral;
  min_bkg = bkg_integral;
  std::vector<double> min_pars(par, par + sizeof(Double_t)*npar);
  seekPool[min_value] = std::vector<float>(min_pars.begin(), min_pars.end());
    std::cout << "Improvement: Sig/sqrt(bkg): " << sig_integral<<"/"<<sqrt(bkg_integral)<< " = " << -f << std::endl;
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
  for(unsigned int i=0;i<pool->functionVars.size(); i++){
    var = pool->functionVars.at(i);
    gMinuit->mnparm(iN  , var, pool->var_start[var],(pool->var_start[var]-pool->var_min[var])/1.01, pool->var_min[var],pool->var_max[var],ierflg);
    iN += 1;
  }
  for(unsigned int i=0;i<pool->doubleVars.size(); i++){
    var = pool->doubleVars.at(i);
    gMinuit->mnparm(iN  , var, pool->var_start[var],(pool->var_start[var]-pool->var_min[var])/1.01, pool->var_min[var],pool->var_max[var],ierflg);
    iN += 1;
  }
}

void PrintBestFit(TMinuit *gMinuit){
  gMinuit->mnprin(1,gMinuit->fAmin);
  std::cout << "Sig/bkg: " << min_sig<<"/"<<min_bkg<< " -> " << -min_value << std::endl;
}
