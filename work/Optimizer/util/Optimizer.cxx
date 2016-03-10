#include <iostream>
#include "Optimizer/computeVar.h"
#include "Optimizer/variablePool.h"
#include "Optimizer/GeneticAlgorithm.h"
#include "TMinuit.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TInterpreter.h"
#include <cmath>
#include <SampleHandler/SampleLocal.h>
#include <SampleHandler/SampleHist.h>
#include "xAODRootAccess/Init.h"
#include "RooStats/NumberCountingUtils.h"

#define SEEK_N 10
#define GA_POOL_SIZE 50
#define GA_MINIM_COUNT 1
#define SEEK_N_TEST 3
#define GA_POOL_SIZE_TEST 4
#define GA_MINIM_COUNT_TEST 4

const char* APP_NAME = "Optimizer";

double (*f)();
void SoverB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void BinomialExpZ(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void getSB(Int_t &npar,  Double_t *par, Double_t *sig_integral, Double_t *bkg_integral, Double_t *bkg_effev=NULL, Double_t *bkg_ev=NULL);
void setStartingPoint(TMinuit *gMinuit);
void PrintBestFit(TMinuit *gMinuit);
float DampMC(float bkg_effev);
variablePool *pool;
Double_t sig_integral, bkg_integral, bkg_ev, bkg_effev, min_sig, min_bkg, min_value=99999;
std::map<float,std::vector<float> > seekPool;
TGraph graph;
int igraph=0;
int evalCount=0;
bool seekon=false;


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
	bool seekrun = false;
  if(argn >= 3 && TString(args[2])=="-s")
    seekrun = true;
    
  gInterpreter->GenerateDictionary("computeVar","computeVar.cxx");
  TString jofile = args[1];
  Options *options = new Options(jofile);
	std::cout << "Parsed options" <<std::endl;
  weight = options->get("weight");
  bkgsyst = options->get("syst").Atof()/100.;
  lumi = options->get("lumi").Atof();


  TString treename = options->get("tree");
	std::vector<TString> bkgpaths = options->getBkgs();
  TFile *sigfile = TFile::Open(options->get("signal"));
  TChain *bkgtree;
  TChain *sigtree;
  if(treename=="sample"){
		std::cout << "Sample reading is tmp broken after including multiple bkg" <<std::endl;
		exit(1);
    //xAOD::Init(APP_NAME);
    //SH::SampleLocal *sample = (SH::SampleLocal *) bkgfile->Get(treename);
    //bkgtree = (TTree *) sample->makeTChain();
    //float xs = sample->getMetaDouble("xs");
    //TFile *bkgfile_hist = TFile::Open(options->get("bkg").ReplaceAll("output-ntuple","hist"));
    //SH::SampleHist *s_hist = (SH::SampleHist *) bkgfile_hist->Get(treename);
    //TH1F *cutflow = (TH1F *) s_hist->readHist("DerivationStat_Weights");
    //int n_tot = cutflow->GetBinContent(1);
    //if(n_tot < 1){
    //  cutflow = (TH1F *) s_hist->readHist("passedWeights_el_Preselection");
    //  n_tot = cutflow->GetBinContent(1);
    //}
    //bkgweight = xs/n_tot;

    //sample = (SH::SampleLocal *) sigfile->Get(treename);
    //sigtree = (TTree *) sample->makeTChain();
    //xs = sample->getMetaDouble("xs");
    //TFile *sigfile_hist = TFile::Open(options->get("signal").ReplaceAll("output-ntuple","hist"));
    //s_hist = (SH::SampleHist *) sigfile_hist->Get(treename);
    //cutflow = (TH1F *) s_hist->readHist("DerivationStat_Weights");
    //n_tot = cutflow->GetBinContent(1);
    //if(n_tot < 1){
    //  cutflow = (TH1F *) s_hist->readHist("passedWeights_el_Preselection");
    //  n_tot = cutflow->GetBinContent(1);
    //}
    //sigweight = xs/n_tot;
  }
  else{
    sigtree = (TChain *) sigfile->Get(treename);
    bkgtree = new TChain(treename);
		for(int b=0;b<bkgpaths.size();b++)
			bkgtree->AddFile(bkgpaths.at(b));
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

  //std::cout << "------- SoverSqrtB -------" << std::endl;
	//void (*theFcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t) = SoverSqrtB;
  std::cout << "------- BinomialExpZ -------" << std::endl;
	void (*theFcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t) = BinomialExpZ;

	
	if(seekrun){
  Int_t ierflg = 0;
  setStartingPoint(gMinuit);
  gMinuit->SetFCN(theFcn);
	seekon = true;
  gMinuit->mnexcm("SEEK", arglist ,2,ierflg);
	seekon = false;
  PrintBestFit(gMinuit);
	}

  //------------ Genetic
  GeneticAlgorithm ga(GA_POOL_SIZE);
  if (testrun)
    ga.SetMaxPool(GA_POOL_SIZE_TEST*testscale);
  ga.SetFCN(N,theFcn);
  std::vector<float> start = pool->GetVarStart();
  std::vector<std::pair<float, int> > steps = pool->GetVarStep();
  std::vector<float> min   = pool->GetVarMin();
  std::vector<float> max   = pool->GetVarMax();
  std::vector<TString> name= pool->GetVarName();
  ga.SetVarMin(min);
  ga.SetVarMax(max);
	ga.SetVarSteps(steps);
	if(seekrun)
  ga.SetInitPool(seekPool);
	else
	ga.Load(options->get("tag"));
  ga.SetVarStart(start);
  if(testrun)
    ga.Minimize(GA_MINIM_COUNT_TEST*testscale);
  else
    ga.Minimize(GA_MINIM_COUNT);
  std::vector<float> lim_min= pool->GetLimMin();
  ga.Dump(options->get("tag"));
  ga.Analyze(name,lim_min);

  // one can try to minimize on top of the best seek
  //arglist[0] = 100*N;
  //ierflg = 0;
  //setStartingPoint(gMinuit);
  //gMinuit->mnexcm("MINIZE", arglist ,2,ierflg);
  //
  //TCanvas can("can");
  //graph.Draw();
  //can.SaveAs("graph.png");
  std::cout << "END: number of evaluations: "<<evalCount <<std::endl;

  return 0;
}

// --- Get integrals
void getSB(Int_t &npar,  Double_t *par, Double_t *sig_integral, Double_t *bkg_integral , Double_t *bkg_effev, Double_t *bkg_ev){

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
  sigtree->Draw("1 >>"+signame,evcut,"goff");
  TH1F *hsig = (TH1F *) gDirectory->Get(signame);
  *sig_integral = lumi*sigweight*hsig->Integral();
  TString bkgname = "hbkg";
  bkgname = bkgname.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","");
  bkgtree->Draw("1 >>"+bkgname,evcut,"goff");
  TH1F *hbkg = (TH1F *) gDirectory->Get(bkgname);
  *bkg_integral = lumi*bkgweight*hbkg->Integral(); //FIXME IntegralAndError
	
	if(bkg_ev)
		*bkg_ev = hbkg->GetEntries();
	if(bkg_effev)
		*bkg_effev = hbkg->GetEffectiveEntries();
	if(*bkg_integral == 0){
		*bkg_effev = 1;
		*bkg_ev = 1;
	}
	evalCount++;
	delete hbkg;
	delete hsig;
//	gDirectory->Clear();
//	gDirectory->DeleteAll();

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
  if(debug){
    std::cout << "Sig/sqrt(bkg): " << sig_integral<<"/"<<sqrt(bkg_integral)<< " = " << -f << std::endl;
  graph.SetPoint(igraph,igraph,-f);
  igraph++;
	}
  
}

void BinomialExpZ(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

	f = 1;
  getSB(npar,par,&sig_integral,&bkg_integral,&bkg_effev,&bkg_ev);
	if(bkg_integral)
  	f = -	(1+RooStats::NumberCountingUtils::BinomialExpZ(sig_integral, bkg_integral, sqrt(bkgsyst*bkgsyst+1./bkg_effev)))*DampMC(bkg_effev);
	else
		f = 9999;
  if (f<min_value){
  min_value = f;
  min_sig = sig_integral;
  min_bkg = bkg_integral;
  	float realZ = RooStats::NumberCountingUtils::BinomialExpZ(sig_integral, bkg_integral, bkgsyst);
    std::cout << "Improvement: 1+BinomialZ with MC stat and damping: (sig/bkg) " << sig_integral<<"/"<<bkg_integral<< " => " << -f << std::endl;
    std::cout << "Improvement: real BinomialZ: " << realZ << std::endl;
    std::cout << "effective MC events: " << bkg_effev << std::endl;
    std::cout << "real MC events     : " << bkg_ev << std::endl;
  } 
	if(iflag && !seekon){
  	float realZ = RooStats::NumberCountingUtils::BinomialExpZ(sig_integral, bkg_integral, bkgsyst);
    std::cout << "FORCED OUTPUT: 1+BinomialZ with MC stat and damping: (sig/bkg) " << sig_integral<<"/"<<bkg_integral<< " => " << -f << std::endl;
    std::cout << "FORCED OUTPUT: real BinomialZ: " << realZ << std::endl;
    std::cout << "FORCED OUTPUT: effective MC events: " << bkg_effev << std::endl;
	}
	if(seekon && ((seekPool.size() < GA_POOL_SIZE) || (f<std::next(seekPool.begin(),GA_POOL_SIZE-1)->first) )){
  	std::vector<double> min_pars(par, par + npar);
  	seekPool[f] = std::vector<float>(min_pars.begin(), min_pars.end());
	}
  if(debug){
    std::cout << "Sig/sqrt(bkg): " << sig_integral<<"/"<<sqrt(bkg_integral)<< " = " << -f << std::endl;
  graph.SetPoint(igraph,igraph,-f);
  igraph++;
	}
  
}

// --- Set starting values and step sizes for parameters
void setStartingPoint(TMinuit *gMinuit){
  int iN = 0;
  Int_t ierflg = 0;
  TString var;
  for(unsigned int i=0;i<pool->functionVars.size(); i++){
    var = pool->functionVars.at(i);
    gMinuit->mnparm(iN  , var, pool->var_start.at(i),(pool->var_start.at(i)-pool->var_min.at(i))/1.01, pool->var_min.at(i),pool->var_max.at(i),ierflg);
    iN += 1;
  }
  for(unsigned int i=0;i<pool->doubleVars.size(); i++){
    var = pool->doubleVars.at(i);
    gMinuit->mnparm(iN  , var, pool->var_start.at(i),(pool->var_start.at(i)-pool->var_min.at(i))/1.01, pool->var_min.at(i),pool->var_max.at(i),ierflg);
    iN += 1;
  }
}

void PrintBestFit(TMinuit *gMinuit){
  gMinuit->mnprin(1,gMinuit->fAmin);
  std::cout << "PrintBestFit - Sig/bkg: " << min_sig<<"/"<<min_bkg<< " -> " << -min_value << std::endl;
}

float DampMC(float bkg_effev){
	return erf(bkg_effev-100)*0.5 + 0.5; //Veto less than 100 MC events
}
