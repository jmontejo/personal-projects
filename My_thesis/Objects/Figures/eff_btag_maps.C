void set_plot_style(){
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}

void eff_btag_maps(){

  int rebinN = 10;
  gROOT->SetBatch(1);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  TCanvas *can = new TCanvas("can","can",700,600);
  can->SetRightMargin(0.15);
  can->SetLeftMargin(0.1);
  set_plot_style();
  TFile *file = TFile::Open("EfficiencyMaps.root");
  
  //eff_b
  TH2F *eff_b = (TH2F *) file->Get("eff_b");
  eff_b->GetXaxis()->SetTitle("Jet |#eta|");
  eff_b->GetYaxis()->SetTitle("Jet p_{T} [GeV]");
  eff_b->GetYaxis()->SetTitleOffset(1.2);
  eff_b->GetZaxis()->SetTitle("B efficiency [%]");
  eff_b->Rebin2D(rebinN,rebinN);
  eff_b->Scale(100./(rebinN*rebinN));
  eff_b->GetYaxis()->SetRangeUser(25,500);
  eff_b->SetMaximum(80.);
  eff_b->SetMinimum(25.);
  eff_b->Draw("colz");
  can->SaveAs("eff_b.eps");
  can->SaveAs("eff_b.pdf");
  can->SaveAs("eff_b.png");
  
  //eff_c
  TH2F *eff_c = (TH2F *) file->Get("eff_c");
  eff_c->GetXaxis()->SetTitle("Jet |#eta|");
  eff_c->GetYaxis()->SetTitle("Jet p_{T} [GeV]");
  eff_c->GetYaxis()->SetTitleOffset(1.2);
  eff_c->GetZaxis()->SetTitle("C efficiency [%]");
  eff_c->Rebin2D(rebinN,rebinN);
  eff_c->Scale(100./(rebinN*rebinN));
  eff_c->GetYaxis()->SetRangeUser(25,500);
  eff_c->SetMaximum(30.);
  eff_c->SetMinimum(0.);
  eff_c->Draw("colz");
  can->SaveAs("eff_c.eps");
  can->SaveAs("eff_c.pdf");
  can->SaveAs("eff_c.png");
  
  //eff_u
  TH2F *eff_u = (TH2F *) file->Get("eff_u");
  eff_u->GetXaxis()->SetTitle("Jet |#eta|");
  eff_u->GetYaxis()->SetTitle("Jet p_{T} [GeV]");
  eff_u->GetYaxis()->SetTitleOffset(1.2);
  eff_u->GetZaxis()->SetTitle("Light efficiency [%]");
  eff_u->Rebin2D(rebinN,rebinN);
  eff_u->Scale(100./(rebinN*rebinN));
  eff_u->GetYaxis()->SetRangeUser(25,500);
  eff_u->SetMaximum(3.);
  eff_u->SetMinimum(0.);
  eff_u->Draw("colz");
  can->SaveAs("eff_u.eps");
  can->SaveAs("eff_u.pdf");
  can->SaveAs("eff_u.png");

}
