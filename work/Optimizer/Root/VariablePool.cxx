#include "TFile.h"
#include "TLeaf.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "Optimizer/ComputeVar.h"
#include "Optimizer/VariablePool.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <utility>

#define DEBUG 1

VariablePool::VariablePool(TTree *sigtree, TTree *bkgtree, Options *options){
	m_cut     	= options->get("cut");
	m_weight  	= options->get("weight");
	m_vars    	= options->getVars();
	m_vars_set  = std::set<TString>(m_vars.begin(),m_vars.end());
	m_vars_step = options->getVarsSteps();
	m_noCheck 	= (options->get("noCheck")=="true");
	m_doPlots   = (options->get("doPlots")=="true");
	const char *envVarContent = getenv("ROOTCOREBIN");
	plotfolder 	= TString(envVarContent)+"/../Optimizer/plots";
	TString commandString = "mkdir -p -m 755 "+plotfolder;
	if(system(commandString.Data()) != 0) std::cout << "^[[31m" << commandString << " failed^[[0m" << std::endl;

	dummy = TFile::Open(plotfolder+"/dummy_"+options->get("tag")+".root","recreate");
	m_sigtree = sigtree->CopyTree(m_cut);
	m_bkgtree = bkgtree->CopyTree(m_cut);
	std::cout << m_cut << m_sigtree << sigtree << std::endl;
	std::cout << "Cut reduced signal tree    : " <<sigtree->GetEntries() << " -> " << m_sigtree->GetEntries() << std::endl;
	std::cout << "Cut reduced background tree: " <<bkgtree->GetEntries() << " -> " << m_bkgtree->GetEntries() << std::endl;
	std::cout << "Building VariablePool..." << std::endl;

	for(unsigned int i=0;i<m_vars.size();i++){
		TString varname = m_vars.at(i);
		float 	step		= m_vars_step.at(i);
		AddVar(varname,step);
	}
	std::cout << "End VariablePool" << std::endl;
}

VariablePool::~VariablePool(){
	dummy->Clear();
}

// --- Add vars
void VariablePool::AddVar(TString varname, float step){

	if(DEBUG) std::cout << "AddVar " << varname << std::endl;

	TString hsigname = "hsig"+varname;
	hsigname = hsigname.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","").ReplaceAll(",","");
	m_sigtree->Draw(varname+">>"+hsigname,m_cut+"*"+m_weight);
	TH1F *hsig = (TH1F *) gDirectory->Get(hsigname);

	TString hbkgname = "hbkg"+varname;
	hbkgname = hbkgname.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","").ReplaceAll(",","");
	TH1F *hbkg = (TH1F *) hsig->Clone(hbkgname);
	m_bkgtree->Draw(varname+">>"+hbkgname,m_cut+"*"+m_weight);

	hsig->Scale(1./hsig->Integral());
	hbkg->Scale(1./hbkg->Integral());

	Variable var(varname,hsig,hbkg,step);
	variables.push_back(var);
	if(m_doPlots)
		doPlot(varname,hsig,hbkg);

	delete hsig;
	delete hbkg;
}


// --- Print content
void VariablePool::Print(){

	std::cout << "--- Variable Pool ---" << std::endl;
	for(unsigned int i=0;i<GetN(); i++){
		variables.at(i).Print();
	}

}

void VariablePool::doPlot(TString var,TH1F *hsig, TH1F *hbkg){
	c1.cd();

	hsig->SetLineColor(2);
	hsig->SetLineWidth(2);
	hbkg->SetLineWidth(2);
	int rebinN = getRebinN(hsig);
	hsig->Rebin(rebinN);
	hbkg->Rebin(rebinN);
	hbkg = (TH1F *) hbkg->DrawNormalized();
	hsig = (TH1F *) hsig->DrawNormalized("same");

	c1.SetLogy(0);
	c1.SaveAs(plotfolder+"/"+var.ReplaceAll("[","").ReplaceAll("]","").ReplaceAll("$","")+".png");
	c1.SetLogy(1);
	c1.SaveAs(plotfolder+"/"+var.ReplaceAll("[","").ReplaceAll("]","").ReplaceAll("$","")+"_logscale.png");

	delete hbkg;
	delete hsig;
}

// --- Get trees
TTree* VariablePool::getSigTree(){ return m_sigtree; }
TTree* VariablePool::getBkgTree(){ return m_bkgtree; }

int VariablePool::getRebinN(TH1F *h){

	TH1F *copy = (TH1F *) h->Clone("copy");
	bool empty = emptyMiddleBins(copy);
	int rb = 1;
	if (empty && copy->GetNbinsX() >= 50 && copy->GetNbinsX()%5==0){
		copy->Rebin(5);
		rb *= 5;
	}
	while (emptyMiddleBins(copy)){
		if(copy->GetNbinsX() <= 10) break;
		if(copy->GetNbinsX()%2==0){copy->Rebin(2); rb*=2;}
		else if(copy->GetNbinsX()%5==0){copy->Rebin(5); rb*=5;}
		else break;
	}

	return rb; 
}

bool VariablePool::emptyMiddleBins(TH1F *h){
	int start=1, end=h->GetNbinsX();
	for(int i=1; i<h->GetNbinsX();i++)
		if(h->GetBinContent(i) > 0){ start=i; break;}
	for(int i=h->GetNbinsX(); i>0;i--)
		if(h->GetBinContent(i) > 0){ end=i; break;}
	for(int i=start; i<end;i++)
		if(h->GetBinContent(i) == 0) return true;
	return false;
}

std::vector<float> VariablePool::GetVarMin(){
	return var_min;
}
std::vector<float> VariablePool::GetLimMin(){
	return lim_min;
}
std::vector<float> VariablePool::GetVarMax(){
	return var_max;
}
std::vector<float> VariablePool::GetLimMax(){
	return lim_max;
}
std::vector<float> VariablePool::GetVarStart(){
	return var_start;
}
std::vector<TString> VariablePool::GetVarName(){
	return m_vars;
}
std::vector<std::pair<float, int> > VariablePool::GetVarStep(){
	return var_step;
}
