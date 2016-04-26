#include "TMath.h"
#include <string>

int significance(){

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  //float xmin = 0.5;
  //float xmax = 2.;
  float xmax = 6;
  float xmin = -2.5;
  float lambda = 9;
  TCanvas *can = new TCanvas("can","can",700,500);
  can->SetTopMargin(0.05);
  can->SetRightMargin(0.02);
  can->SetLeftMargin(0.1);
  TF1 *fgaus = new TF1("fgaus","1./(sqrt(2*TMath::Pi()))*( exp(-pow(x,2)/2.) )",xmin,xmax);
  fgaus->SetLineColor(2);
  fgaus->SetLineWidth(3);

  fgaus->Draw();
  fgaus->GetHistogram()->GetXaxis()->SetTitle("x");
  fgaus->GetHistogram()->GetYaxis()->SetTitle("Probability density");
  fgaus->GetHistogram()->GetYaxis()->SetTitleOffset(1.5);

  TLine *line = new TLine();
  line->SetLineWidth(2);
  line->DrawLine(1.64,0,1.64,0.15);
  line->DrawLine(3.00,0,3.00,0.15);
  line->DrawLine(5.00,0,5.00,0.15);
  //TLegend *leg = new TLegend(0.6,0.63,0.88,0.84);
  //leg->SetLineColor(0);
  //leg->AddEntry(fgaus,"Gaussian prior","l");
  //leg->AddEntry(flog,"Log-normal prior","l");
  //leg->AddEntry(fgamma,"Gamma prior","l");
  //leg->Draw("same");
  TLatex *tex = new TLatex();
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->DrawLatex(1.64,0.17,"Z=1.64");
  tex->DrawLatex(3   ,0.17,"Z=3");
  tex->DrawLatex(5   ,0.17,"Z=5");
  tex->SetNDC(1);
  tex->DrawLatex(0.60,0.88,"J. Montejo, PhD Thesis");
  can->SaveAs("significance.eps");
}
