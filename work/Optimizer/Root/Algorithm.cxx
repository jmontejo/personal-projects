#include "Optimizer/Algorithm.h"
#include "Optimizer/SeekAlgorithm.h"
#include "RooStats/NumberCountingUtils.h"
#include "TObjArray.h"
#include "TROOT.h"
#include <vector>
#include <iostream>

//--- static vars
static Algorithm *Algorithm_obj;

Algorithm::Algorithm(VariablePool *vars){
	Algorithm_obj = this;
	debug = false;
	this->vars = vars;
  N = vars->GetN();
  gMinuit = new TMinuit(N);
	bool debug = false;
  if(debug) gMinuit->SetPrintLevel(1);
  else      gMinuit->SetPrintLevel(-1);
 	gMinuit->SetFCN(Algorithm::BinomialExpZ_forMinuit);
 	//gMinuit->SetObjectFit(&tmp_result);
// create minimizer giving a name and a name (optionally) for the specific
// algorithm
// possible choices are: 
//     minName                  algoName
// Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
//  Minuit2                     Fumili2
//  Fumili
//  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS, 
//                              BFGS2, SteepestDescent
//  GSLMultiFit
//   GSLSimAn
//   Genetic
	minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2");
// set tolerance , etc...
	functor =  new ROOT::Math::Functor(this,&Algorithm::BinomialExpZ_forMinimizer,N); 
	minimizer->SetFunction(*functor);
	minimizer->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2 
  minimizer->SetMaxIterations(10000);  // for GSL 
  minimizer->SetTolerance(0.001);
  minimizer->SetPrintLevel(1);
  setStartingPoint();
}

Algorithm* Algorithm::GetAlgorithm(TString name, TString options, VariablePool *vars){

	auto tokens = options.Tokenize(":");
	std::vector<TString> voptions;
	for(int i=0; i<tokens->GetEntries(); i++)
		voptions.push_back(*((TString *)tokens->At(i)));
	if(name=="Seek")
		return new SeekAlgorithm(voptions,vars);
	else{
		std::cout << "Algorithm not defined: " << name <<std::endl;
		exit(1);
	}
}

//void Algorithm::SetFunction(	void(* fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t) ){
//	theFcn = fcn;
//}


//void PrintBestFit(TMinuit *gMinuit){
//  gMinuit->mnprin(1,gMinuit->fAmin);
//  std::cout << "PrintBestFit - Sig/bkg: " << min_sig<<"/"<<min_bkg<< " -> " << -min_value << std::endl;
//}


float Algorithm::DampMC(float bkg_effev){
	return erf(bkg_effev-100)*0.5 + 0.5; //Veto less than 100 MC events
}

void Algorithm::checkMetric(float val, float sig, float bkg, std::vector<double> max_pars){
  if (val <= max_value) return;
  max_value = val;
  max_sig = sig;
  max_bkg = bkg;
//  if(debug){
//    std::cout << "Sig/sqrt(bkg): " << sig_integral<<"/"<<sqrt(bkg_integral)<< " = " << val << std::endl;
//  graph.SetPoint(igraph,igraph,val);
//  igraph++;
//	}
//  std::vector<double> min_pars(par, par + sizeof(Double_t)*npar);
//  seekPool[min_value] = std::vector<float>(min_pars.begin(), min_pars.end());
//    std::cout << "Improvement: Sig/sqrt(bkg): " << sig_integral<<"/"<<sqrt(bkg_integral)<< " = " << val << std::endl;
//  min_value = val;
//  min_sig = sig_integral;
//  min_bkg = bkg_integral;
//  	float realZ = RooStats::NumberCountingUtils::BinomialExpZ(sig_integral, bkg_integral, bkgsyst);
//    std::cout << "Improvement: 1+BinomialZ with MC stat and damping: (sig/bkg) " << sig_integral<<"/"<<bkg_integral<< " => " << val << std::endl;
//    std::cout << "Improvement: real BinomialZ: " << realZ << std::endl;
//    std::cout << "effective MC events: " << bkg_effev << std::endl;
//    std::cout << "real MC events     : " << bkg_ev << std::endl;
//  } 
//	if(iflag && !seekon){
//  	float realZ = RooStats::NumberCountingUtils::BinomialExpZ(sig_integral, bkg_integral, bkgsyst);
//    std::cout << "FORCED OUTPUT: 1+BinomialZ with MC stat and damping: (sig/bkg) " << sig_integral<<"/"<<bkg_integral<< " => " << val << std::endl;
//    std::cout << "FORCED OUTPUT: real BinomialZ: " << realZ << std::endl;
//    std::cout << "FORCED OUTPUT: effective MC events: " << bkg_effev << std::endl;
//	}
//	if(seekon && ((seekPool.size() < GA_POOL_SIZE) || (val<std::next(seekPool.begin(),GA_POOL_SIZE-1)->first) )){
//  	std::vector<double> min_pars(par, par + npar);
//  	seekPool[val] = std::vector<float>(min_pars.begin(), min_pars.end());
//	}

}


void Algorithm::SetBkgSyst(double s){ bkgsyst = s; }

// --- Figures of merit
void Algorithm::SoverB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

	Double_t sig_integral, bkg_integral;
  getSB(par,&sig_integral,&bkg_integral);
  f = -sig_integral/(bkg_integral+0.00001); 
}

void Algorithm::SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

	Double_t sig_integral, bkg_integral;
  getSB(par,&sig_integral,&bkg_integral);
  f = -sig_integral/sqrt(bkg_integral+pow(bkg_integral*bkgsyst,2)+1);
  
}

void Algorithm::BinomialExpZ_forMinuit(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
	f = Algorithm_obj->BinomialExpZ(par);
}
float Algorithm::BinomialExpZ_forMinimizer(const Double_t *par){
	std::cout << "In BinomialExpZ_forMinimizer" << std::endl;
	return Algorithm_obj->BinomialExpZ(par);
}

float Algorithm::BinomialExpZ(const Double_t *par){
	std::cout << "In BinomialExpZ" << std::endl;

	float f = 1;
	Double_t sig_integral, bkg_integral, bkg_effev, bkg_ev;
  getSB(par,&sig_integral,&bkg_integral,&bkg_effev,&bkg_ev);
	if(bkg_integral)
  	f = -	(1+RooStats::NumberCountingUtils::BinomialExpZ(sig_integral, bkg_integral, sqrt(bkgsyst*bkgsyst+1./bkg_effev)))*DampMC(bkg_effev);
	else
		f = 9999;
	return f;
  
}

// --- Get integrals
void Algorithm::getSB(const Double_t *par, Double_t *sig_integral, Double_t *bkg_integral , Double_t *bkg_effev, Double_t *bkg_ev){

  TString evcut    = "1";
  int iN = 0;
  for(unsigned int i=0;i<vars->GetN(); i++){
    auto var = vars->variables.at(i);
    evcut += Form("*(%s > %g)",var.name.Data(),par[iN]);
    iN  += 1;
  }
  evcut += "*"+weight;
  if(debug)
    std::cout << evcut << std::endl;

  TTree *sigtree = vars->getSigTree();
  TTree *bkgtree = vars->getBkgTree();

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
	delete hbkg;
	delete hsig;
//	gDirectory->Clear();
//	gDirectory->DeleteAll();

}

// --- Set starting values and step sizes for parameters
void Algorithm::setStartingPoint(){
  Int_t ierflg = 0;
  for(unsigned int i=0;i<vars->GetN(); i++){
    auto var = vars->variables.at(i);
    gMinuit->mnparm(i , var.name, var.f_getStart(), ( var.f_getStart()-var.f_getMin())/1.01, var.f_getMin(), var.f_getMax(),ierflg);
		if(debug)
			std::cout << Form("%d %s %f %f %f %f %d",i , var.name.Data(), var.f_getStart(), ( var.f_getStart()-var.f_getMin())/1.01, var.f_getMin(), var.f_getMax(),ierflg) <<std::endl;
		minimizer->SetLimitedVariable(i , Form("%d",i), var.f_getStart(), ( var.f_getStart()-var.f_getMin())/1.01, var.f_getMin(), var.f_getMax());
		
  }
}

void Algorithm::SetParameters(TString weight, float lumi, float bkgsyst, float bkgweight, float sigweight){
	this->weight		= weight;	
	this->lumi 			= lumi;	
	this->bkgsyst 	= bkgsyst;	
	this->bkgweight = bkgweight;	
	this->sigweight = sigweight;	
}

