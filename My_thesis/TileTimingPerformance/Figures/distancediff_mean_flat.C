{
//=========Macro generated from canvas: c_center/c_center
//=========  (Wed Mar 28 09:43:26 2012) by ROOT version5.28/00c
   TCanvas *c_center = new TCanvas("c_center", "c_center",187,167,700,480);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c_center->Range(-625,-2.725,625,2.525);
   c_center->SetFillColor(10);
   c_center->SetBorderMode(0);
   c_center->SetBottomMargin(0.105);
   c_center->SetBorderSize(2);
   c_center->SetFrameFillColor(0);
   c_center->SetFrameBorderMode(0);
   c_center->SetFrameBorderMode(0);
   Double_t xAxis1[51] = {-500, -480, -460, -440, -420, -400, -380, -360, -340, -320, -300, -280, -260, -240, -220, -200, -180, -160, -140, -120, -100, -80, -60, -40, -20, 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 420, 440, 460, 480, 500}; 
   
   TH1D *line = new TH1D("line","CENTER_time_truedist_sample3_gain1_dupl0",50, xAxis1);
   line->SetBinContent(10,-0.8944113);
   line->SetBinContent(11,-0.6658241);
   line->SetBinContent(12,-0.4467569);
   line->SetBinContent(13,-0.3074598);
   line->SetBinContent(14,-0.3006726);
   line->SetBinContent(15,-0.1773874);
   line->SetBinContent(16,-0.1979802);
   line->SetBinContent(17,-0.09686004);
   line->SetBinContent(18,-0.08929986);
   line->SetBinContent(19,-0.08629168);
   line->SetBinContent(20,-0.0401695);
   line->SetBinContent(21,-0.02002631);
   line->SetBinContent(22,-0.05831213);
   line->SetBinContent(23,0.03774805);
   line->SetBinContent(24,-0.002213771);
   line->SetBinContent(25,-0.01965339);
   line->SetBinContent(26,-0.08208161);
   line->SetBinContent(27,-0.1372478);
   line->SetBinContent(28,-0.1308621);
   line->SetBinContent(29,-0.02627787);
   line->SetBinContent(30,-0.05541769);
   line->SetBinContent(31,-0.0414225);
   line->SetBinContent(32,-0.001628324);
   line->SetBinContent(33,0.1599699);
   line->SetBinContent(34,0.226477);
   line->SetBinContent(35,0.2777232);
   line->SetBinContent(36,0.2729044);
   line->SetBinError(10,0.0514579);
   line->SetBinError(11,0.033477);
   line->SetBinError(12,0.0257044);
   line->SetBinError(13,0.0228814);
   line->SetBinError(14,0.0212106);
   line->SetBinError(15,0.0210053);
   line->SetBinError(16,0.0189686);
   line->SetBinError(17,0.0146753);
   line->SetBinError(18,0.0124101);
   line->SetBinError(19,0.0147685);
   line->SetBinError(20,0.0131171);
   line->SetBinError(21,0.012409);
   line->SetBinError(22,0.0130773);
   line->SetBinError(23,0.00706789);
   line->SetBinError(24,0.00649137);
   line->SetBinError(25,0.00296799);
   line->SetBinError(26,0.00273559);
   line->SetBinError(27,0.00697976);
   line->SetBinError(28,0.0080623);
   line->SetBinError(29,0.0120932);
   line->SetBinError(30,0.0142695);
   line->SetBinError(31,0.0145861);
   line->SetBinError(32,0.0123283);
   line->SetBinError(33,0.0121388);
   line->SetBinError(34,0.0150311);
   line->SetBinError(35,0.0239885);
   line->SetBinError(36,0.048578);
   line->SetMinimum(-2.2);
   line->SetMaximum(2);
   line->SetEntries(54);
   line->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   line->SetLineColor(ci);
   line->GetXaxis()->SetTitle("Distance to center [mm]");
   line->GetXaxis()->SetLabelFont(42);
   line->GetXaxis()->SetLabelSize(0.050);
   line->GetXaxis()->SetTitleSize(0.050);
   line->GetXaxis()->SetTitleFont(42);
   line->GetYaxis()->SetTitle("Time_{corrected} - Mean time [ns]");
   line->GetYaxis()->SetLabelFont(42);
   line->GetYaxis()->SetLabelSize(0.050);
   line->GetYaxis()->SetTitleSize(0.050);
   line->GetYaxis()->SetTitleFont(42);
   line->GetZaxis()->SetLabelFont(42);
   line->GetZaxis()->SetLabelSize(0.050);
   line->GetZaxis()->SetTitleSize(0.050);
   line->GetZaxis()->SetTitleFont(42);
   line->Draw("");
   c_center->Modified();
   c_center->cd();
   c_center->SetSelected(c_center);
}
