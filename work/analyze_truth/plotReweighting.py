import ROOT as root

prefix = "rw"
samples = {"ttB":"rw_q1_eta_rw_q1_pt",
           "ttb":"rw_q1_eta_rw_q1_pt",
           "ttbb":"rw_qq_dr_rw_qq_pt",}

vars = ["ttbar_pt","top_pt","qq_dr","qq_pt","qq_m","qq_ht","q1_pt","q1_eta"]

rfile = root.TFile.Open("analyze_truth_outfiles/HFsystematics2DSherpa_p15.root")
  
root.gROOT.SetBatch(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetOptStat(0)
c0  =root.TCanvas("c0")
canv=root.TCanvas( "canv", "canv",800,800)
pad_1=root.TPad("pad_1", "up", 0., 0.35, 1., 1.)
pad_1.SetBottomMargin(0)
pad_1.Draw()
pad_2=root.TPad("pad_2", "down", 0.0, 0.00, 1.0, 0.35)
pad_2.SetTopMargin(0)
pad_2.SetBottomMargin(0.30)
pad_2.Draw()
tex = root.TLatex()
tex.SetTextSize(0.06)
tex.SetTextFont(42)
tex.SetTextAlign(12)

for varname in vars:
  for sample, rw in samples.iteritems():
    namerw = "hist_Nominal_%s_%s_%s" % (sample, varname, rw)
    if namerw not in [x.GetName() for x in rfile.GetListOfKeys()]:
        print "Not in file", namerw
        continue
    name1 = "hist_Nominal_%s_%s" % (sample, varname)
    name2 = "hist_Nominal_%s_%s2" % (sample, varname)
    hrw = rfile.Get(namerw)
    h   = rfile.Get(name1)
    h2  = rfile.Get(name2)
    h.GetYaxis().SetTitleSize(0.06)
    h.GetYaxis().SetTitleOffset(0.75)
    h.GetYaxis().SetLabelSize(0.050)
    h.GetYaxis().SetTitle("Arbitrary units")
    h.GetYaxis().SetTitleSize(0.06)
    h.GetYaxis().SetTitleOffset(0.70)
    h.GetYaxis().SetLabelSize(0.050)
    h.SetLineWidth(3)

    h.SetLineColor(1)
    h.SetMarkerColor(1)
    h2.SetLineColor(2)
    h2.SetMarkerColor(2)
    hrw.SetLineColor(2)
    hrw.SetMarkerColor(2)
    hrw.SetLineStyle(2)

    h.SetMarkerStyle(4)
    pad_1.cd()
    #print h.Integral(0,-1)
    h.SetMaximum(h.GetMaximum()*1.8)
    if not "dr" in varname and not "eta" in varname: 
      h.SetMaximum(h.GetMaximum()*3)
    else:
      h.SetMinimum(10e-3)
    if "pt" in varname or "HT" in varname or "M" in varname: 
      h.SetMaximum(h.GetMaximum()*3)
    if "extHFtype" in varname:
      h.SetMaximum(h.GetMaximum()*10)
      h.SetMinimum(2e-4)
    h.DrawCopy()
    h2.DrawCopy("same")
    hrw.DrawCopy("same")
    leg = root.TLegend(0.45,0.61,0.89,0.88)
    leg.SetFillStyle(0)
    leg.SetLineColor(0)
    leg.AddEntry(h,"Sherpa+OpenLoops")
    leg.AddEntry(h2,"Powheg+Pythia")
    leg.AddEntry(hrw,"Powheg+Pythia reweighted")
    pad_2.cd()
    hratio = h2.Clone() 
    hratiorw = hrw.Clone() 
    hratio.Divide(h)
    hratiorw.Divide(h)
    hratio.SetMaximum(1.999)
    hratio.SetMinimum(0.001)
    hratio.GetYaxis().SetTitle("MC / Sherpa+OpenLoops")
    xtitle = hratio.GetXaxis().GetTitle()
    if "pT" in xtitle: xtitle = xtitle.replace("pT","p_{T}")
    if "p_{T}" in xtitle and not "GeV" in xtitle: xtitle += " (GeV)"
    xtitle = xtitle.replace("(GeV) (GeV)","(GeV)")
    print xtitle
    hratio.GetXaxis().SetTitle(xtitle)
    hratio.GetYaxis().SetNdivisions(504)
    #hratio.GetYaxis().SetTitle("MC / SHERPA OL default")
    #hratio.SetMaximum(0.99) #FIXME
    #hratio.SetMinimum(0.01)
    if "extHFtype" in varname:
      hratio.GetXaxis().SetLabelSize(0.13)
    else:
      hratio.GetXaxis().SetLabelSize(0.1)
    hratio.GetYaxis().SetLabelSize(0.08)
    hratio.GetXaxis().SetTitleSize(0.1)
    hratio.GetXaxis().SetLabelOffset(0.015)
    hratio.GetXaxis().SetTitleOffset(1.2)
    hratio.GetYaxis().SetTitleSize(0.085)
    hratio.GetYaxis().SetTitleOffset(0.60)
    hratio.DrawCopy()
    hratiorw.DrawCopy("same")
    line = root.TLine(hratio.GetBinLowEdge(1),1,hratio.GetBinLowEdge(hratio.GetNbinsX()+1),1)
    line.SetLineWidth(2)
    line.Draw()
    if not "dr" in varname and not "eta" in varname: 
      pad_1.SetLogy(1);
    else:
      pad_1.SetLogy(0);
    #end sample
    pad_1.cd()
    tex.DrawLatexNDC(0.14,0.85,"#font[72]{ATLAS} Simulation")
    tex.SetTextSize(0.05)
    tex.DrawLatexNDC(0.14,0.76,"J. Montejo, PhD Thesis")
    #leg.AddEntry(root.NULL,"(*) MPI-FSR not split","")
    leg.SetTextSize(0.045)
    leg.Draw()
    plotname = "_".join([prefix,sample,varname])
    plotname = plotname.replace("ttB","tt1gbbq")
    plotname = plotname.replace("ttb_","tt1bq_")
    #canv.SaveAs("plots_tesis/"+plotname+".pdf")
    #canv.SaveAs("plots_tesis/"+plotname+".pdf")
    canv.SaveAs("plots_tesis/"+plotname+".eps")
    canv.SaveAs("plots_tesis/"+plotname+".pdf")
