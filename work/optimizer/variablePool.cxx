#include "TFile.h"
#include "TLeaf.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "computeVar.h"
#include "variablePool.h"
#include <iostream>
#include <set>

#define DEBUG 1

variablePool::variablePool(TTree *sigtree, TTree *bkgtree, Options *options){
  m_cut     = options->getCut();
  m_weight  = options->getWeight();
  m_vars    = options->getVars();
  m_noCheck = options->getNoCheck();
  TFile *dummy = TFile::Open("dummy.root","recreate");
  std::cout << "Building variablePool..." << std::endl;
  m_sigtree = sigtree->CopyTree(m_cut);
  m_bkgtree = bkgtree->CopyTree(m_cut);

  GetVarsFromTrees();
  std::cout << "End variablePool" << std::endl;
  AddFunctionVar("computeVar::Diff(jet_btag_weight[3],jet_btag_weight[2])");
  AddFunctionVar("computeVar::BtagN(jet_btag_weight,3)");
  //AddFunctionVar("computeVar::BtagN(jet_MV1c_weight,3)");
  //AddFunctionVar("computeVar::BtagN(jet_btag_weight,2)+computeVar::BtagN(jet_btag_weight,3)");
  //AddFunctionVar("computeVar::BtagN(jet_MV1c_weight,2)+computeVar::BtagN(jet_MV1c_weight,3)");

}

// --- Add function, double, int
void variablePool::AddFunctionVar(TString var){
  int start = var.Index("(")+1;
  int end   = var.Index(")");
  if(start==-1 || end==-1){
    std::cout << "Function variable without (): " << var << std::endl;
    exit(1); 
  }
  TString args = var(start,end-start);
  TObjArray *tokens = args.Tokenize(",");
  if(!m_noCheck)
  for(int i=0;i<tokens->GetEntries();i++)
    if(var_min.find( ((TObjString *) tokens->At(i))->GetString() ) == var_min.end()){
      std::cout << "Function uses a variable that is not in the variable pool: " << ((TObjString *) tokens->At(i))->GetString() << std::endl;
      exit(1);
    }
  AddVar(var,&functionVars);
}
void variablePool::AddVectorVar(TString var){
  if(!var.Contains("jet_pt")) return;
//  TCanvas c1;
//  TString signame = "hsig"+var;
//  m_sigtree->Draw("@"+var+".size() >>"+signame,m_cut);
//  TH1F *hsig = (TH1F *) gDirectory->Get(signame);
//  c1.SaveAs(var+"_size.png");
//  TString bkgname = "hbkg"+var;
//  m_bkgtree->Draw("@"+var+".size() >>"+bkgname,m_cut);
//  TH1F *hbkg = (TH1F *) gDirectory->Get(bkgname);
//  int max = 0, max2 =0;
//  for(int i=hbkg->GetNbinsX();i>0;i--)
//    if(hbkg->GetBinContent(i)!=0){
//      max = hbkg->GetBinCenter(i);
//      break;
//    }
//  for(int i=hsig->GetNbinsX();i>0;i--)
//    if(hsig->GetBinContent(i)!=0){
//      max2 = hsig->GetBinCenter(i);
//      break;
//    }
//  max = std::min(max,max2);
//  max = std::min(max,5);
  int max=3; //HACK
  TString vararray;
  for(int i=0;i<=max;i++){
    vararray = Form("%s[%d]",var.Data(),i);
    AddVar(vararray,&doubleVars);
  }
}
void variablePool::AddDoubleVar(TString var){
  AddVar(var,&doubleVars);
}
void variablePool::AddIntVar(TString var){
  AddVar(var,&intVars);
}

// --- Add vars
void variablePool::AddVar(TString var,std::vector<TString> *vec){

  if(DEBUG) std::cout << "AddVar " << var << std::endl;
  vec->push_back(var);

  TString signame = "hsig"+var;
  signame = signame.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","").ReplaceAll(",","");
  std::cout << TString(var+">>"+signame+"(1000)") << std::endl;
  m_sigtree->Draw(var+">>"+signame+"(1000)",m_cut+"*"+m_weight);
  std::cout << TString(var+">>"+signame+"(1000)") << std::endl;
  TH1F *hsig = (TH1F *) gDirectory->Get(signame);
  std::cout << TString(var+">>"+signame+"(1000)") << std::endl;
  TString bkgname = "hbkg"+var;
  bkgname = bkgname.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","");
  m_bkgtree->Draw(var+">>"+bkgname+"(1000)",m_cut+"*"+m_weight);
  TH1F *hbkg = (TH1F *) gDirectory->Get(bkgname);
  //---- hack
  //TH1F *hsig2;
  //TH1F *hbkg2;
  //if (var.Contains("jet_pt[")){
  //  int index = var.Index("[");
  //  float ptcut = 40000-5000*TString(var(index+1,1)).Atoi();
  //  TString cut = Form(" && %s >= %f",var.Data(), ptcut);
  //  m_sigtree->Draw(var+">>"+signame+"2(1000)","TRFMCweight_in[4]*LeptonSF*MCweightXS*(jet_n>=6 "+cut+")");
  //  hsig2 = (TH1F *) gDirectory->Get(signame+"2");
  //  m_bkgtree->Draw(var+">>"+bkgname+"2(1000)","TRFMCweight_in[4]*LeptonSF*MCweightXS*(jet_n>=6 "+cut+")");
  //  hbkg2 = (TH1F *) gDirectory->Get(bkgname+"2");
  //  std::cout << "Cut " << var << " " << ptcut << std::endl;
  //  std::cout.precision(15);
  //  std::cout << "Efficiency sig: " <<  hsig2->Integral(0,-1)/hsig->Integral(0,-1) << std::endl;
  //  std::cout << "Efficiency bkg: " <<  hbkg2->Integral(0,-1)/hbkg->Integral(0,-1) << std::endl;
  //}
  //---- hack
  hsig->Scale(1./hsig->Integral());
  hbkg->Scale(1./hbkg->Integral());
  doPlot(var,hsig,hbkg);


  float minsig, maxsig, minbkg, maxbkg;
  for(int i=0;i<hsig->GetNbinsX();i++)
    if(hsig->GetBinContent(i)!=0){
      minsig = hsig->GetBinLowEdge(i);
      break;
    }
  for(int i=0;i<hbkg->GetNbinsX();i++)
    if(hbkg->GetBinContent(i)!=0){
      minbkg = hbkg->GetBinLowEdge(i);
      break;
    }
  for(int i=hbkg->GetNbinsX();i>0;i--)
    if(hbkg->GetBinContent(i)!=0){
      maxbkg = hbkg->GetBinLowEdge(i);
      break;
    }
  for(int i=hsig->GetNbinsX();i>0;i--)
    if(hsig->GetBinContent(i)!=0){
      maxsig = hsig->GetBinLowEdge(i);
      break;
    }
  var_min[var] = std::max(minsig,minbkg);
  var_max[var] = std::min(hsig->GetMean(),hbkg->GetMean());
  //var_max[var] = std::min(maxsig,maxbkg);
  lim_min[var] = std::min(minsig,minbkg);
  lim_max[var] = std::max(maxsig,maxbkg);
  var_mean[var] = (hsig->GetMean()+var_min[var]*2)/3.;
  //if (var.Contains("jet_pt[")){
  //  int index = var.Index("[");
  //  int ptcut = 40000-5000*TString(var(index+1,1)).Atoi();
  //  var_mean[var] = std::max(ptcut,26000);
  //}

  delete hsig, hbkg;
}

// --- Get list of variables from trees
void variablePool::GetVarsFromTrees(){
  std::set<TString> sig_set;
  TObjArray *sig_array = m_sigtree->GetListOfBranches();
  for(int i=0; i<sig_array->GetEntries(); i++)
    sig_set.insert( sig_array->At(i)->GetName() ); //FIXME, need to determine the type of the variable, assume double for all
  std::set<TString> bkg_set;
  TObjArray *bkg_array = m_bkgtree->GetListOfBranches();
  for(int i=0; i<bkg_array->GetEntries(); i++)
    bkg_set.insert( bkg_array->At(i)->GetName() );

  std::map<TString,TString> var_map;
  TObjArray *leaves_array = m_bkgtree->GetListOfLeaves();
  for(int i=0; i<leaves_array->GetEntries(); i++){
    var_map[leaves_array->At(i)->GetName()] = ((TLeaf*)(leaves_array->At(i)))->GetTypeName();
    std::cout << leaves_array->At(i)->GetName() << " " << ((TLeaf*)(leaves_array->At(i)))->GetTypeName() << std::endl;  
  }

  TString basetypes   = "float double Float_t Double_t";
  TString vectortypes = "vector<float> vector<double> vector<Float_t> vector<Double_t>";

  for(int i=0;i<m_vars.size();i++){
    TString varname = m_vars.at(i);
    std::cout << varname <<std::endl;
    if(m_noCheck || bkg_set.count(varname)){
      if(basetypes.Contains(var_map[varname]))
        AddDoubleVar(varname);
      else if(vectortypes.Contains(var_map[varname]))
        AddVectorVar(varname);
      else{
        std::cout << "This variable type is not being treated yet: " << varname << " " << var_map[varname] << std::endl;
        if(m_noCheck)
          AddDoubleVar(varname);
      }
    }
  }
}

// --- Print content
void variablePool::Print(){

  TString var;
  std::cout << "--- Variable Pool ---" << std::endl;
  std::cout << "--- Functions: "<< functionVars.size() << std::endl;
  for(int i=0;i<functionVars.size(); i++){
    var = functionVars.at(i);
    std::cout << var << "\t" << var_min[var] <<"\t" << var_max[var] << "\t| " << lim_min[var] <<"\t" << lim_max[var] << std::endl;
  }
  std::cout << "--- Doubles: "<< doubleVars.size() << std::endl;
  for(int i=0;i<doubleVars.size(); i++){
    var = doubleVars.at(i);
    std::cout << var << "\t" << var_min[var] <<"\t" << var_max[var] << "\t| " << lim_min[var] <<"\t" << lim_max[var] << std::endl;
  }

}

void variablePool::doPlot(TString var,TH1F *hsig, TH1F *hbkg){
  c1.cd();
  
  hsig->SetLineColor(2);
  hsig->SetLineWidth(2);
  hbkg->SetLineWidth(2);
  hbkg->DrawNormalized();
  hsig->DrawNormalized("same");
  c1.SaveAs(var.ReplaceAll("[","").ReplaceAll("]","")+".png");
  c1.SetLogy(1);
  c1.SaveAs(var.ReplaceAll("[","").ReplaceAll("]","")+"_logscale.png");
}

// --- Get trees
TTree* variablePool::getSigTree(){ return m_sigtree; }
TTree* variablePool::getBkgTree(){ return m_bkgtree; }
