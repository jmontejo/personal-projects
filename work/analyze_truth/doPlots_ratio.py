import ROOT as root
import array, os

root.gROOT.SetBatch(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetOptStat(0)

py_lepdata = [3.39923,6.70447,5.16093,2.72501,1.18613,0.350055,0.0200267]
her_lepdata = [3.4813,6.7152,5.1155,2.6765,1.1485,0.3320,0.0190]
toppt_bins = array.array("d",[0,50,100,150,200,250,350,800])

vars = (
  #"add_bjet_n_4jex",
  #"add_bjet_n_5jex",
  #"add_bjet_n_6jin",
  #"jet_n",
  #"bjet_n",
  #"add_bjet_n",
  #"lep_pt",
  #"lep_eta",
  #"leadjet_pt",
  #"leadjet_eta",
  #"ht",
  #"bjet_n_4jin",
  #"bjet_n_4jex",
  #"bjet_n_5jex",
  #"bjet_n_6jin",
  #"HFtype",
  "top_pt_binned",
  #"top_eta",
  "ttbar_pt_binned",
  #"ttbar_eta",
  #"ttbar_pt",
  #"top_pt",
  #"aplanarity",
  #"centrality",
  #"leadjet_pt_4jex",
  #"leadjet_pt_5jex",
  #"leadjet_pt_6jin",
  #"leadjet_pt_4jin",
  #"ht_4jex",
  #"ht_5jex",
  #"ht_6jin",
  #"ht_4jin",
  #"centrality_4jex",
  #"centrality_5jex",
  #"centrality_6jin",
  #"centrality_4jin",
  #"aplanarity_4jex",
  #"aplanarity_5jex",
  #"aplanarity_6jin",
  #"aplanarity_4jin",
  #"maxdeltaeta",
  #"maxdeltaeta_4jex",
  #"maxdeltaeta_5jex",
  #"maxdeltaeta_6jin",
  #"maxdeltaeta_4jin",

)

all_samples = ("AlpgenHFOR","PowhegPythiaRw","PowhegPythia","Sherpa","MadgraphKtfac2","Madgraph5F","PowhegHerwig")
alpgen_samples = ("AlpgenHFOR","AlpgenHFtype","AlpgenKtfac2","AlpgenKtfac2Rw","PowhegPythiaRw")
best_samples = ("AlpgenHFOR","AlpgenKtfac2","MadgraphKtfac2","PowhegPythiaRw","Sherpa2","PowhegHerwig")
bestrw_samples = ("AlpgenKtfac2Rw","MadgraphKtfac2Rw","PowhegPythiaRw","PowhegHerwigRw")
bestnorw_samples = ("AlpgenKtfac2","MadgraphKtfac2","PowhegPythiaRw","PowhegPythia","PowhegHerwig")
pythia_samples = ("Sherpa","MadgraphKtfac2","Madgraph5F","PowhegPythia","Data")
herwig_samples = ("AlpgenKtfac2Rw","Data")
andrea_samples = ("Madgraph_CT10","Madgraph_CTEQ6")
madgraph_samples = ("PowhegPythiaRw","Madgraph_CT10","Madgraph_CTEQ6","MadgraphKtfac2","MadgraphCMS_Z2","MadgraphCMS_Perugia","Madgraph_Q2up_radLo","Madgraph_Q2up_modPerugia")
madgraph_q2up_samples = ("PowhegPythiaRw","Madgraph_CT10","MadgraphKtfac2","Madgraph_Q2up_radLo","Madgraph_Q2up_modPerugia")
madgraphrw_samples = ("PowhegPythiaRw","Madgraph_CT10","Madgraph_CTEQ6Rw","MadgraphKtfac2Rw","Madgraph5FRw")
powpy_samples = ["PowhegPythia_CT10_"+x for x in ("nom","facsc05","facsc2","rensc05","rensc2")]
powher_samples = ["PowhegJimmy_CT10_"+x for x in ("nom","facsc05","facsc2","rensc05","rensc2")]

comparisons = {
                #"madgraphrw":madgraphrw_samples,
                #"madgraph":madgraph_samples,
                #"madgraph_q2up":madgraph_q2up_samples,
                #"all":all_samples,
                #"best":best_samples,
                #"alpgen":alpgen_samples,
                #"andrea":andrea_samples,
                #"pythia":pythia_samples,
                #"Powheg_Pythia":powpy_samples,
                #"Powheg_Herwig":powher_samples,
              }

rename = {
          #"Madgraph5F"    : "CTEQ6, q2nom, Perugia, pdfrw=true",
          "Madgraph0a"    : "CTEQ6, pdfrw=false, CMS gridpack, Madspin",
          "Madgraph0a"    : "CTEQ6, pdfrw=false, CMS gridpack, Madspin",
          "Madgraph0a_noTau"    : "CTEQ6, pdfrw=false, CMS gridpack, Madspin, no taus",
          "Madgraph0b"    : "CTEQ6, pdfrw=false",
          "Madgraph1b"    : "CTEQ6, pdfrw=true, CMS gridpack, no Madspin",
          "Madgraph2b"    : "CTEQ6, pdfrw=false, Valerio gridpack, no Madspin",
          "Madgraph3a"    : "CTEQ6, pdfrw=false, CMS gridpack, Madspin, massless B",
          "Madgraph3b"    : "CTEQ6, pdfrw=false, massless B",
          "Madgraph4a"    : "CT10, pdfrw=false, CMS gridpack, Madspin",
          "Madgraph4b"    : "CT10, pdfrw=false",
          "MadgraphCTEQ6down"   : "CTEQ6, pdfrw=false, Q2 up",
          "MadgraphCT10pdftrue" : "CT10, pdfrw=true, CMS gridpack, no Madspin",
          "Madgraph_CT10" : "CT10, q2nom, Perugia, pdfrw=false",
          "Madgraph_CTEQ6": "CTEQ6, q2nom, Perugia, pdfrw=false",
          "Madgraph_masslessB": "CTEQ6, q2nom, Perugia, pdfrw=false, massless B",
          "MadgraphCMS_Perugia": "CTEQ6, q2nom, Perugia, pdfrw=false",
          "MadgraphCMS_Z2": "CTEQ6, q2nom, Z2, pdfrw=false",
          "MadgraphKtfac2": "CTEQ6, q2 UP, Z2, pdfrw=true",
          "Madgraph_Q2up_radLo": "CTEQ6, q2 UP, radLo, pdfrw=true",
          "Madgraph_Q2up_modPerugia": "CTEQ6, q2 UP, modPerugia, pdfrw=true",
         }

tune_samples  = ("MadgraphCMS_Z2","MadgraphCMS_Perugia")
gridpack_samples = ("Madgraph_CTEQ6","MadgraphCMS_Perugia")
pdf_samples   = ("Madgraph_CTEQ6","Madgraph_CT10")
# pack_samples  = ("Madgraph_CTEQ6","Madgraph5F")
# scale_samples = ("MadgraphKtfac2","Madgraph5F")
mass_samples   = ("Madgraph0b","Madgraph3b")
mass_samples   = ("Madgraph0b","Madgraph3b")
madgraphskim_samples = ("PowhegPythiaRw","Madgraph_CT10","Madgraph_CTEQ6","MadgraphKtfac2","Madgraph_Q2up_radLo","Madgraph_masslessB")
madgraphXX_samples = ("PowhegPythiaRw","Madgraph0a","Madgraph0b","Madgraph1b","Madgraph3a","Madgraph3b","Madgraph4a","Madgraph4b")
madgraphXXskim_samples = ("PowhegPythiaRw","Madgraph4b","MadgraphCTEQ6down")
madgraphXXnew_samples = ("PowhegPythiaRw","MadgraphCT10up","MadgraphCTEQ6down")
madgraphrw_samples = ("PowhegPythiaRw","Madgraph4bRw","MadgraphCTEQ6downRw")
madgraphrw2_samples = ("PowhegPythiaRw","Madgraph4bRw","MadgraphCTEQ6downRw","Madgraph4b","MadgraphCTEQ6down")
madgraphfinal_samples = ("PowhegPythiaRw","Madgraph4bRw","MadgraphFinalRw")
madgraph_dilep_samples = ("PowhegPythiaRw","Madgraph0a","Madgraph0a_noTau","Madgraph0b")
off_samples = ("PowhegPythiaMiniTruth_full","MadgraphOff_nominal","MadgraphOff_nominal_rw","PowhegHerwig","PowhegHerwig_rw")
# 
comparisons = {
                "off": off_samples,
                #"tune": tune_samples,
                #"gridpack":gridpack_samples,
                #"mass":  mass_samples,
                #"pdf":  pdf_samples,
                #"pack": pack_samples,
                #"scale":scale_samples,
                #"madgraph":madgraph_samples,
                #"madgraphskim":madgraphskim_samples,
                #"madgraph_q2up":madgraph_q2up_samples,
                #"best":  best_samples,
                #"madgraphfix":  madgraphXX_samples,
                #"madgraphfixnew":  madgraphXXnew_samples,
                #"madgraphfixskim":  madgraphXXskim_samples,
                #"madgraphrw2":  madgraphrw2_samples,
                #"madgraphfinal":  madgraphfinal_samples,
                #"madgraphrw":  madgraphrw_samples,
                #"madgraphfix_dilep":  madgraph_dilep_samples,
                #"aMcAtNlo": aMcAtNlo_samples,
                #"bestrw":bestrw_samples,
                #"bestnorw":bestnorw_samples,
              }

infile = root.TFile.Open("analyze_truth_outfiles/outhistos.root")
canv=root.TCanvas( "canv", "canv",800,800)
pad_1=root.TPad("pad_1", "up", 0., 0.35, 1., 1.)
pad_1.SetBottomMargin(0)
pad_1.Draw()
pad_2=root.TPad("pad_2", "down", 0.0, 0.00, 1.0, 0.35)
pad_2.SetTopMargin(0)
pad_2.SetBottomMargin(0.25)
pad_2.Draw()

for compname, samples in comparisons.iteritems():
  if not os.path.exists("plots/"+compname):
    os.mkdir("plots/"+compname)
  for varname in vars:
    hmap = dict()
    leg = root.TLegend(0.4,0.5,0.89,0.89)
    #leg.SetHeader(seltitle)
    leg.SetFillColor(0)
    leg.SetLineColor(0)
    #leg.SetTextFont(92)
    #leg.SetTextSize(9)
    for i,sample in enumerate(samples):
  
      hname = varname+"_"+sample
      if "Data" in sample:
        if not "top_pt_binned" in varname: continue
        h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",len(toppt_bins)-1,toppt_bins)
        if "pythia" in compname:
          for i,bin in enumerate(py_lepdata):
            h.SetBinContent(i+1,bin/1000.)
        elif "herwig" in compname:
          for i,bin in enumerate(her_lepdata):
            h.SetBinContent(i+1,bin/1000.)
      else:
        h = infile.Get(hname)
      print hname, h
  
      if "maxdeltaeta" in varname or "lep" in varname:
        h.Rebin(2)
      if "jet_eta" in varname:
        h.GetXaxis().SetRangeUser(0,2.5)
      hmap[hname] = h
      if i==4: i=len(samples)
      h.SetLineColor(i+1)
      h.SetLineWidth(3)
      h.SetLineStyle(1)
      #if "Mbbover2" in sample or "Sherpa" in sample: h.SetLineStyle(2)
  
      mode = "h e1"
      if (i!=0): mode += " same"
      pad_1.cd()
      h.SetMaximum(h.GetMaximum()*1.4)
      if sample.replace("Rw","") in rename.keys() :
        if "Rw" in sample: 
          h.SetTitle(rename[sample.replace("Rw","")]+", reweighted")
        else:
          h.SetTitle(rename[sample])
      else:
        h.SetTitle(sample)

      h.GetYaxis().SetTitleOffset(0.5)
      h.GetYaxis().SetTitleSize(0.06)
      h.GetYaxis().SetLabelSize(0.06)
      h.Draw(mode)
      leg.AddEntry(h)
    #end for samples
    leg.Draw()
    pad_2.cd()

    for i,( hname, h) in enumerate(hmap.iteritems()):
      hratio = h.Clone(hname+"_ratio")
      if "Powheg_Pythia" in compname:
        hratio.Divide(hmap[varname+"_PowhegPythia_CT10_nom"])
      elif "Powheg_Herwig" in compname:
        hratio.Divide(hmap[varname+"_PowhegJimmy_CT10_nom"])
      elif "andrea" in compname:
        hratio.Divide(hmap[varname+"_Madgraph_CT10"])
      elif varname+"_PowhegPythiaRw" in hmap.keys():
        hratio.Divide(hmap[varname+"_PowhegPythiaRw"])
      else: 
        hratio.Divide(hmap[varname+"_"+samples[0]])
  
      if i==0:
        if "bjet_n" in varname: 
          hratio.SetMaximum(2)
          hratio.SetMinimum(0)
        else:
          hratio.SetMaximum(1.5)
          hratio.SetMinimum(0.5)
        hratio.GetYaxis().SetNdivisions(507)
        hratio.GetYaxis().SetTitle("MC/PowhegPythia")
        hratio.GetYaxis().SetTitleOffset(0.45)
        hratio.GetYaxis().SetTitleSize(0.11)
        hratio.GetYaxis().SetLabelSize(0.10)
        hratio.GetXaxis().SetTitleOffset(0.8)
        hratio.GetXaxis().SetTitleSize(0.12)
        hratio.GetXaxis().SetLabelSize(0.11)
        hratio.DrawCopy()
      else:
        hratio.DrawCopy("same")
      del hratio
    pad_1.SetLogy(0)
    canv.SaveAs("plots/"+compname+"/"+varname+".png")
    canv.SaveAs("plots/"+compname+"/"+varname+".eps")
    canv.SaveAs("plots/"+compname+"/"+varname+".pdf")
    pad_1.SetLogy(1)
    canv.SaveAs("plots/"+compname+"/"+varname+"_logscale.png")
    canv.SaveAs("plots/"+compname+"/"+varname+"_logscale.eps")
    canv.SaveAs("plots/"+compname+"/"+varname+"_logscale.pdf")
    for h in hmap.items(): del h
  #end for vars
infile.Close()
