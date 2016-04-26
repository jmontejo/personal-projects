#include "TMath.h"

int test_statistic(bool histo=true, TString hyp1="#mu", TString hyp2="#mu'"){

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  float xmin = 1e-10;
  float xmax = 30;
  float ymin = 5e-4;
  float ymax = 2;
  float lambda = 9;
  TCanvas *can = new TCanvas("can","can",800,600);
  can->SetTopMargin(0.05);
  TF1 *f0 = new TF1("f0","1./(2*sqrt(x*2*TMath::Pi()))*( exp(-pow((sqrt(x)+sqrt([0])),2)/2.)+exp(-pow((sqrt(x)-sqrt([0])),2)/2.) )",xmin,xmax);
  f0->SetParameter(0,0);
  f0->SetLineColor(2);
  f0->SetLineWidth(2);
  TF1 *f0fill = new TF1("f0fill","1./(2*sqrt(x*2*TMath::Pi()))*( exp(-pow((sqrt(x)+sqrt([0])),2)/2.)+exp(-pow((sqrt(x)-sqrt([0])),2)/2.) )",lambda,xmax);
  f0fill->SetParameter(0,0);
  f0fill->SetLineColor(2);
  f0fill->SetLineWidth(2);
  f0fill->SetFillColor(kRed-7);
  f0fill->SetFillStyle(1001);
  TH1F *hb = new TH1F("hb","hb",100,xmin,xmax);
  hb->FillRandom("f0",1e4);
  hb->Scale(1./(hb->GetBinWidth(1)*hb->Integral()));
  hb->SetLineColor(2);
  //for (int i=1;i<30;i++)
  //  std::cout <<i << " " << f->Integral(xmin,i) << std::endl;

  TF1 *f1 = new TF1("f1","1./(2*sqrt(x*2*TMath::Pi()))*( exp(-pow((sqrt(x)+sqrt([0])),2)/2.)+exp(-pow((sqrt(x)-sqrt([0])),2)/2.) )",xmin,xmax);
  f1->SetParameter(0,lambda);
  f1->SetLineColor(4);
  f1->SetLineWidth(2);
  TH1F *hsb = new TH1F("hsb","hsb",100,xmin,xmax);
  hsb->FillRandom("f1",1e4);
  hsb->Scale(1./(hsb->GetBinWidth(1)*hsb->Integral()));
  hsb->SetLineColor(4);
  f0->SetMinimum(ymin);
  f0->SetMaximum(ymax);
  f0->Draw();
  f0->GetHistogram()->GetXaxis()->SetTitle("q_{"+hyp1+"}");
  f0->GetHistogram()->GetYaxis()->SetTitle("Probability density");
  f1->Draw("same");
  if(histo){
    hsb->Draw("same");
    hb->Draw("same");
  }
  else
    f0fill->Draw("BCSAME");

  TLegend *leg = new TLegend(0.7,0.7,0.84,0.84);
  leg->SetLineColor(0);
  leg->AddEntry(f0,"f(q_{"+hyp1+"}| "+hyp1+")","l");
  leg->AddEntry(f1,"f(q_{"+hyp1+"}| "+hyp2+")","l");
  leg->Draw("same");
  
  TLatex *tex = new TLatex();
  tex->SetTextFont(42);
  if(!histo){
  tex->DrawLatex(6,0.5,"Median[q_{"+hyp1+"}| "+hyp2+"]");
  tex->DrawLatex(11,3e-3,"Expected p_{"+hyp1+"}");
  TLine *line = new TLine(lambda,ymin,lambda,0.3);
  line->SetLineWidth(2);
  line->Draw();
  TArrow *ar2 = new TArrow(11,2.5e-3,10.2,1e-3,0.02,"|>");
  ar2->SetAngle(40);
  ar2->SetLineWidth(2);
  //ar2->SetLineColor(2);
  ar2->Draw();
  }
  tex->SetTextSize(0.04);
  tex->DrawLatex(18,1,"J. Montejo, PhD Thesis");
  can->SetLogy(1);
  TString histstr = histo? "1":"0";

  can->SaveAs("pvalue_"+hyp1+"_"+hyp2+"_hist"+histstr+".eps");
}
