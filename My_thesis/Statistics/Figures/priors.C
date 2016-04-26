#include "TMath.h"
#include <string>

int priors(float sigma){

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  //float xmin = 0.5;
  //float xmax = 2.;
  float xmin = 1e-10;
  float xmax = 2.5;
  float ymin = 5e-4;
  float ymax = 2;
  float lambda = 9;
  TCanvas *can = new TCanvas("can","can",800,700);
  can->SetTopMargin(0.05);
  can->SetRightMargin(0.02);
  can->SetLeftMargin(0.08);
  TF1 *fgaus = new TF1("fgaus","1./([0]*sqrt(2*TMath::Pi()))*( exp(-pow((x-1)/[0],2)/2.) )",xmin,xmax);
  fgaus->SetParameter(0,sigma);
  fgaus->SetLineColor(2);
  fgaus->SetLineWidth(2);
  TF1 *flog = new TF1("flog","1./(x*log([0])*sqrt(2*TMath::Pi()))*( exp(-pow(log(x)/log([0]),2)/2.) )",xmin,xmax);
  flog->SetParameter(0,1+sigma);
  flog->SetLineColor(3);
  flog->SetLineWidth(2);
  TF1 *fgamma = new TF1("fgamma","pow([0]-1,[0])*pow(x,[0]-1)*exp(-([0]-1)*x)/TMath::Gamma([0])",xmin,xmax);
  fgamma->SetParameter(0,1./(sigma*sigma));
  fgamma->SetLineColor(4);
  fgamma->SetLineWidth(2);

  flog->Draw();
  fgaus->Draw("same");
  fgamma->Draw("same");
  flog->GetHistogram()->GetXaxis()->SetTitle("x");
  flog->GetHistogram()->GetYaxis()->SetTitle("Probability density");

  TLegend *leg = new TLegend(0.6,0.63,0.88,0.84);
  leg->SetLineColor(0);
  leg->AddEntry(fgaus,"Gaussian prior","l");
  leg->AddEntry(flog,"Log-normal prior","l");
  leg->AddEntry(fgamma,"Gamma prior","l");
  leg->Draw("same");
  TLatex *tex = new TLatex();
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->SetNDC(1);
  tex->DrawLatex(0.57,0.88,"J. Montejo, PhD Thesis");
  can->SaveAs("prior"+TString(Form("%.2f",sigma))+".eps");
}
