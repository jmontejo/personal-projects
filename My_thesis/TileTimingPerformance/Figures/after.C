{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Nov  8 10:26:04 2011) by ROOT version5.28/00c
   TCanvas *c1 = new TCanvas("c1", "c1",700,500);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c1->Range(-25.595,-1480.264,25.755,11955.97);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetBottomMargin(0.1101695);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TH1F *htemp__2__1__1__1 = new TH1F("htemp__2__1__1__1","",100,-41,38);
   htemp__2__1__1__1->SetBinContent(8,1);
   htemp__2__1__1__1->SetBinContent(23,1);
   htemp__2__1__1__1->SetBinContent(24,2);
   htemp__2__1__1__1->SetBinContent(26,1);
   htemp__2__1__1__1->SetBinContent(29,1);
   htemp__2__1__1__1->SetBinContent(30,1);
   htemp__2__1__1__1->SetBinContent(31,1);
   htemp__2__1__1__1->SetBinContent(33,2);
   htemp__2__1__1__1->SetBinContent(34,2);
   htemp__2__1__1__1->SetBinContent(36,4);
   htemp__2__1__1__1->SetBinContent(37,4);
   htemp__2__1__1__1->SetBinContent(38,6);
   htemp__2__1__1__1->SetBinContent(39,4);
   htemp__2__1__1__1->SetBinContent(40,11);
   htemp__2__1__1__1->SetBinContent(41,16);
   htemp__2__1__1__1->SetBinContent(42,30);
   htemp__2__1__1__1->SetBinContent(43,42);
   htemp__2__1__1__1->SetBinContent(44,101);
   htemp__2__1__1__1->SetBinContent(45,195);
   htemp__2__1__1__1->SetBinContent(46,452);
   htemp__2__1__1__1->SetBinContent(47,1092);
   htemp__2__1__1__1->SetBinContent(48,2389);
   htemp__2__1__1__1->SetBinContent(49,4680);
   htemp__2__1__1__1->SetBinContent(50,7482);
   htemp__2__1__1__1->SetBinContent(51,9686);
   htemp__2__1__1__1->SetBinContent(52,10107);
   htemp__2__1__1__1->SetBinContent(53,8300);
   htemp__2__1__1__1->SetBinContent(54,5681);
   htemp__2__1__1__1->SetBinContent(55,3323);
   htemp__2__1__1__1->SetBinContent(56,1610);
   htemp__2__1__1__1->SetBinContent(57,778);
   htemp__2__1__1__1->SetBinContent(58,379);
   htemp__2__1__1__1->SetBinContent(59,170);
   htemp__2__1__1__1->SetBinContent(60,98);
   htemp__2__1__1__1->SetBinContent(61,51);
   htemp__2__1__1__1->SetBinContent(62,27);
   htemp__2__1__1__1->SetBinContent(63,18);
   htemp__2__1__1__1->SetBinContent(64,9);
   htemp__2__1__1__1->SetBinContent(65,7);
   htemp__2__1__1__1->SetBinContent(66,5);
   htemp__2__1__1__1->SetBinContent(67,1);
   htemp__2__1__1__1->SetBinContent(68,2);
   htemp__2__1__1__1->SetBinContent(70,1);
   htemp__2__1__1__1->SetBinContent(71,1);
   htemp__2__1__1__1->SetBinContent(74,1);
   htemp__2__1__1__1->SetBinContent(93,1);
   htemp__2__1__1__1->SetEntries(56776);
   htemp__2__1__1__1->Scale(1./10000);
   htemp__2__1__1__1->SetDirectory(0);
   htemp__2__1__1__1->SetStats(0);
   
   TPaveStats *ptstats = new TPaveStats(0.6795977,0.7194093,0.8807471,0.8797468,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *text = ptstats->AddText("D6 time distribution");
   text->SetTextSize(0.03687764);
   text = ptstats->AddText("Entries = 56776  ");
   text = ptstats->AddText("Mean  = -0.5179");
   text = ptstats->AddText("RMS   =  1.855");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   htemp__2__1__1__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(htemp__2__1__1__1->GetListOfFunctions());

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   htemp__2__1__1__1->SetLineColor(ci);
   htemp__2__1__1__1->GetXaxis()->SetTitle("Cell time [ns]");
   htemp__2__1__1__1->GetXaxis()->SetRange(27,78);
   htemp__2__1__1__1->GetXaxis()->SetLabelFont(42);
   htemp__2__1__1__1->GetXaxis()->SetLabelSize(0.050);
   htemp__2__1__1__1->GetXaxis()->SetTitleSize(0.050);
   htemp__2__1__1__1->GetXaxis()->SetTitleFont(42);
   htemp__2__1__1__1->GetYaxis()->SetTitle("Arbitrary units");
   htemp__2__1__1__1->GetYaxis()->SetLabelFont(42);
   htemp__2__1__1__1->GetYaxis()->SetLabelSize(0.050);
   htemp__2__1__1__1->GetYaxis()->SetTitleSize(0.050);
   htemp__2__1__1__1->GetYaxis()->SetTitleOffset(1);
   htemp__2__1__1__1->GetYaxis()->SetTitleFont(42);
   htemp__2__1__1__1->GetZaxis()->SetLabelFont(42);
   htemp__2__1__1__1->GetZaxis()->SetLabelSize(0.050);
   htemp__2__1__1__1->GetZaxis()->SetTitleSize(0.050);
   htemp__2__1__1__1->GetZaxis()->SetTitleFont(42);
   htemp__2__1__1__1->Draw("");
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->SaveAs("after.eps");
}
