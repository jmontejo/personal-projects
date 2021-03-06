{
//=========Macro generated from canvas: c1/c1
//=========  (Mon Nov  7 11:24:41 2011) by ROOT version5.28/00c
   //TCanvas *c1 = new TCanvas("c1", "c1",25,324,700,502);
   TCanvas *c1 = new TCanvas("c1", "c1",700,500);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(-1892.5,-0.3,22432.5,2.7);
   c1->SetFillColor(10);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetBottomMargin(0.105);
   c1->SetFrameFillColor(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   Double_t xAxis1[23] = {540, 740, 940, 1140, 1340, 1540, 1740, 1940, 2140, 2340, 2540, 2740, 2940, 3140, 3340, 3564, 3836, 4208, 4712, 5472, 6728, 9172, 20000}; 
   
   TH1D *SIGMA_time_ene_sample3_gain1_dupl0 = new TH1D("SIGMA_time_ene_sample3_gain1_dupl0","SIGMA_time_ene_sample3_gain1_dupl0",22, xAxis1);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(1,2.12752);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(2,1.8221);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(3,1.60053);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(4,1.46767);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(5,1.38408);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(6,1.31746);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(7,1.25178);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(8,1.19981);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(9,1.14862);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(10,1.11609);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(11,1.04916);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(12,1.03758);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(13,0.992823);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(14,0.944723);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(15,0.924492);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(16,0.928581);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(17,0.885664);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(18,0.885498);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(19,0.818249);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(20,0.768014);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(21,0.729676);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinContent(22,0.664649);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(1,0.00474797);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(2,0.00376545);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(3,0.0032109);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(4,0.00328284);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(5,0.00388617);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(6,0.00498626);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(7,0.00612317);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(8,0.00759428);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(9,0.00923643);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(10,0.0111073);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(11,0.0124394);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(12,0.0130932);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(13,0.0148459);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(14,0.0158127);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(15,0.0155483);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(16,0.0162947);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(17,0.0153302);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(18,0.0147354);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(19,0.0140802);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(20,0.0138147);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(21,0.0138177);
   SIGMA_time_ene_sample3_gain1_dupl0->SetBinError(22,0.0120193);
   SIGMA_time_ene_sample3_gain1_dupl0->SetMinimum(0);
   SIGMA_time_ene_sample3_gain1_dupl0->SetMaximum(2.4);
   SIGMA_time_ene_sample3_gain1_dupl0->SetEntries(22);
   SIGMA_time_ene_sample3_gain1_dupl0->SetStats(0);
   
   TF1 *fitres = new TF1("fitres","sqrt([0]*[0]+([1]/sqrt((x/1000)))*([1]/sqrt((x/1000)))+([2]/(x/1000))*([2]/(x/1000)))",0,29000);
   fitres->SetFillColor(19);
   fitres->SetFillStyle(0);
   fitres->SetLineColor(2);
   fitres->SetLineWidth(2);
   fitres->SetChisquare(133.92);
   fitres->SetNDF(19);
   fitres->SetParameter(0,0.548608);
   fitres->SetParError(0,0.010406);
   fitres->SetParLimits(0,0,0);
   fitres->SetParameter(1,1.4101);
   fitres->SetParError(1,0.0119332);
   fitres->SetParLimits(1,0,0);
   fitres->SetParameter(2,0.664736);
   fitres->SetParError(2,0.0168901);
   fitres->SetParLimits(2,0,0);
   SIGMA_time_ene_sample3_gain1_dupl0->GetListOfFunctions()->Add(fitres);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   SIGMA_time_ene_sample3_gain1_dupl0->SetLineColor(ci);
   SIGMA_time_ene_sample3_gain1_dupl0->GetXaxis()->SetTitle("E_{cell} [MeV]");
   SIGMA_time_ene_sample3_gain1_dupl0->GetXaxis()->SetLabelFont(42);
   SIGMA_time_ene_sample3_gain1_dupl0->GetXaxis()->SetLabelSize(0.050);
   SIGMA_time_ene_sample3_gain1_dupl0->GetXaxis()->SetTitleSize(0.050);
   SIGMA_time_ene_sample3_gain1_dupl0->GetXaxis()->SetTitleFont(42);
   SIGMA_time_ene_sample3_gain1_dupl0->GetXaxis()->SetNdivisions(1004);
   SIGMA_time_ene_sample3_gain1_dupl0->GetYaxis()->SetTitle("#sigma(t_{cell}) [ns]");
   SIGMA_time_ene_sample3_gain1_dupl0->GetYaxis()->SetNdivisions(505);
   SIGMA_time_ene_sample3_gain1_dupl0->GetYaxis()->SetLabelFont(42);
   SIGMA_time_ene_sample3_gain1_dupl0->GetYaxis()->SetLabelSize(0.050);
   SIGMA_time_ene_sample3_gain1_dupl0->GetYaxis()->SetTitleSize(0.050);
   SIGMA_time_ene_sample3_gain1_dupl0->GetYaxis()->SetTitleFont(42);
   SIGMA_time_ene_sample3_gain1_dupl0->GetZaxis()->SetLabelFont(42);
   SIGMA_time_ene_sample3_gain1_dupl0->GetZaxis()->SetLabelSize(0.050);
   SIGMA_time_ene_sample3_gain1_dupl0->GetZaxis()->SetTitleSize(0.050);
   SIGMA_time_ene_sample3_gain1_dupl0->GetZaxis()->SetTitleFont(42);
   SIGMA_time_ene_sample3_gain1_dupl0->Draw("");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->SaveAs("e_res.eps");
}
