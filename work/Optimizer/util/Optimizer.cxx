#include <iostream>
#include "Optimizer/ComputeVar.h"
#include "Optimizer/VariablePool.h"
#include "Optimizer/GeneticAlgorithm.h"
#include "Optimizer/OptimizationPoint.h"
#include "Optimizer/Algorithm.h"
#include "TMinuit.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TInterpreter.h"
#include <cmath>
#include <SampleHandler/SampleLocal.h>
#include <SampleHandler/SampleHist.h>
#include "xAODRootAccess/Init.h"

#define SEEK_N 10
#define GA_POOL_SIZE 50
#define GA_MINIM_COUNT 1
#define SEEK_N_TEST 3
#define GA_POOL_SIZE_TEST 4
#define GA_MINIM_COUNT_TEST 4

const char* APP_NAME = "Optimizer";

double (*f)();
void PrintBestFit(TMinuit *gMinuit);
float DampMC(float bkg_effev);
TString GetTreeName(TString treename, TString filepath);
TChain* GetTreeFromSample(TString samplename);
TChain* GetTreeFromSampleList(std::vector<TString> bkgpaths);
VariablePool *pool;
Double_t sig_integral, bkg_integral, bkg_ev, bkg_effev, min_sig, min_bkg, min_value=99999;
std::map<float,std::vector<float> > seekPool;
TGraph graph;
int igraph=0;
int evalCount=0;
bool seekon=false;


bool debug = 0;
TString weight, displayweight;
float lumi=1., bkgsyst=0.;

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
    
  gInterpreter->GenerateDictionary("ComputeVar","ComputeVar.cxx");
  TString jofile = args[1];
  Options *options = new Options(jofile);
	if(testrun) options->set("doPlots","false");
	std::cout << "Parsed options" <<std::endl;
  weight = options->get("weight");
  displayweight = options->get("displayweight");
  bkgsyst = options->get("syst").Atof()/100.;
  lumi = options->get("lumi").Atof();


  TString treename = options->get("tree");
	std::vector<TString> bkgpaths = options->getBkgs();
  TFile *sigfile = TFile::Open(options->get("signal"));
  TChain *bkgchain;
  TChain *sigtree;
  if(treename=="sample"){
    xAOD::Init(APP_NAME);
    sigtree  = GetTreeFromSample(options->get("signal"));
    bkgchain = GetTreeFromSampleList(bkgpaths);
  }
  else{
    sigtree = (TChain *) sigfile->Get(GetTreeName(treename,sigfile->GetName()));
    bkgchain = new TChain(treename);
		for(int b=0;b<bkgpaths.size();b++){
			bkgchain->AddFile(bkgpaths.at(b),-1,GetTreeName(treename,bkgpaths.at(b)));
			//bkgchain->AddFile(bkgpaths.at(b),TTree::kMaxEntries,GetTreeName(treename,bkgpaths.at(b)));
		}
  }

  pool = new VariablePool(sigtree,bkgchain,options);
  pool->Print();
	OptimizationPoint::SetVariablePool(pool);

	std::vector<Algorithm *> algorithms;
	auto alg_jo = options->getAlgorithms();
	for(auto alg = alg_jo.begin(); alg != alg_jo.end(); alg++){
		algorithms.push_back(Algorithm::GetAlgorithm((*alg).first,(*alg).second,pool));
	}

	TCanvas c;
  TMultiGraph *mg = new TMultiGraph();
  mg->SetTitle("Peformance graphs");
	for(int i=0;i<algorithms.size();i++){
		auto alg = algorithms.at(i);
		std::cout << "Running Algorithm: " << alg->name <<std::endl;
		alg->SetParameters(weight,lumi,bkgsyst,1.,1.,displayweight);
		alg->Execute();
		c.cd();
		auto g = alg->getGraph();
		g->SetLineColor(i+1);
		g->SetMarkerColor(i+1);
		g->SetMarkerStyle(20);
		mg->Add(g);
		auto gr = alg->getGraphRounded();
		gr->SetLineColor(i+1);
		gr->SetLineStyle(2);
		gr->SetMarkerColor(i+1);
		gr->SetMarkerStyle(22);
		mg->Add(gr);
	}
	mg->Draw("ALP");
	c.SetLogx();
	c.SaveAs("perfgraph.png");

//
//
//
//
//  //------------ Genetic
//  GeneticAlgorithm ga(GA_POOL_SIZE);
//  if (testrun)
//    ga.SetMaxPool(GA_POOL_SIZE_TEST*testscale);
//  ga.SetFCN(N,theFcn);
//  std::vector<float> start = pool->GetVarStart();
//  std::vector<std::pair<float, int> > steps = pool->GetVarStep();
//  std::vector<float> min   = pool->GetVarMin();
//  std::vector<float> max   = pool->GetVarMax();
//  std::vector<TString> name= pool->GetVarName();
//  ga.SetVarMin(min);
//  ga.SetVarMax(max);
//	ga.SetVarSteps(steps);
//	if(seekrun)
//  ga.SetInitPool(seekPool);
//	else
//	ga.Load(options->get("tag"));
//  ga.SetVarStart(start);
//  if(testrun)
//    ga.Minimize(GA_MINIM_COUNT_TEST*testscale);
//  else
//    ga.Minimize(GA_MINIM_COUNT);
//  std::vector<float> lim_min= pool->GetLimMin();
//  ga.Dump(options->get("tag"));
//  ga.Analyze(name,lim_min);
//
//  // one can try to minimize on top of the best seek
//  //arglist[0] = 100*N;
//  //ierflg = 0;
//  //setStartingPoint(gMinuit);
//  //gMinuit->mnexcm("MINIZE", arglist ,2,ierflg);
//  //
//  //TCanvas can("can");
//  //graph.Draw();
//  //can.SaveAs("graph.png");
//  std::cout << "END: number of evaluations: "<<evalCount <<std::endl;

  return 0;
}

TString GetTreeName(TString treename, TString filepath){
	if (treename!="export") return treename;
	filepath = filepath(0,filepath.Last('/'));
	int where = filepath.Last('/')+1;
	treename =  filepath(where,filepath.Length())+"_Nom";
	return treename;
}

TChain* GetTreeFromSample(TString samplename){
  TFile *file = TFile::Open(samplename);
  SH::SampleLocal *sample = (SH::SampleLocal *) file->Get("sample");
  TChain *chain = sample->makeTChain();
  float xs = sample->getMetaDouble("xs");
  TFile *file_hist = TFile::Open(samplename.ReplaceAll("output-ntuple","hist"));
  SH::SampleHist *s_hist = (SH::SampleHist *) file_hist->Get("sample");
  TH1F *cutflow = (TH1F *) s_hist->readHist("DerivationStat_Weights");
  int n_tot = cutflow->GetBinContent(1);
  if(n_tot < 1){
    cutflow = (TH1F *) s_hist->readHist("passedWeights_el_Preselection");
    n_tot = cutflow->GetBinContent(1);
  }
  float weight = xs/n_tot;
	TChain *newchain = (TChain*) chain->CloneTree(0); 
	newchain->Branch("xs_weight", &weight, "xs_weight/F"); 
	for( int i=0; i < chain->GetEntries(); i++){ 
		chain->GetEntry(i); 
		newchain->Fill();
	}
	return newchain;
} 

TChain* GetTreeFromSampleList(std::vector<TString> bkgpaths){
	TTree *newchain;
	float xs_weight;
	TFile *dummy = TFile::Open("dummy.root","recreate");
	for(int b=0;b<bkgpaths.size();b++){
  	TFile *file = TFile::Open(bkgpaths.at(b));
  	SH::SampleLocal *sample = (SH::SampleLocal *) file->Get("sample");
  	TChain *chain = sample->makeTChain();
		if(b==0){
			dummy->cd();
			newchain = chain->CloneTree(0);
			newchain->Branch("xs_weight", &xs_weight, "xs_weight/F"); 
		}
  	float xs = sample->getMetaDouble("xs");
  	TFile *file_hist = TFile::Open(bkgpaths.at(b).ReplaceAll("output-ntuple","hist"));
  	SH::SampleHist *s_hist = (SH::SampleHist *) file_hist->Get("sample");
  	TH1F *cutflow = (TH1F *) s_hist->readHist("DerivationStat_Weights");
  	int n_tot = cutflow->GetBinContent(1);
  	if(n_tot < 1){
  	  cutflow = (TH1F *) s_hist->readHist("passedWeights_el_Preselection");
  	  n_tot = cutflow->GetBinContent(1);
  	}
  	float xs_weight = xs/n_tot;
		std::cout << bkgpaths.at(b) << std::endl;
		std::cout << xs << " " << n_tot << std::endl;
		for( int i=0; i < chain->GetEntries(); i++){ 
			chain->GetEntry(i); 
			newchain->Fill();
		}
		std::cout << newchain->GetEntries() << std::endl;
		file->Close();
		delete chain;
	}
	newchain->SetDirectory(0);
	newchain->AutoSave("SaveSelf");
	return ((TChain *) newchain);
} 
