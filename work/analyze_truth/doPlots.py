import ROOT as root
import array
import pickle

root.gROOT.SetBatch(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetOptStat(0)
try:
  rwdict = pickle.load(open("MGrw.pkl"))
except EOFError:
  print "couldn't load MGrw.pkl"

outfile = root.TFile.Open("analyze_truth_outfiles/outhistos.root","update")

vars = {
        #"add_bjet_n_4jex": "Sum$(jet_trueflav==5 && jet_id==0)",
        #"add_bjet_n_5jex": "Sum$(jet_trueflav==5 && jet_id==0)",
        #"add_bjet_n_6jin": "Sum$(jet_trueflav==5 && jet_id==0)",
        #"maxdeltaeta" :"maxdeltaeta",
        #"maxdeltaeta_4jex" :"maxdeltaeta",
        #"maxdeltaeta_5jex" :"maxdeltaeta",
        #"maxdeltaeta_6jin" :"maxdeltaeta",
        #"maxdeltaeta_4jin" :"maxdeltaeta",
        #"jet_n" :"jet_n",
        #"bjet_n":"Sum$(int(jet_trueflav)==5)",
        #"add_bjet_n": "Sum$(jet_trueflav==5 && jet_id==0)",
        #"lep_pt":"lep_pt/1000.",
        #"lep_eta":"abs(lep_eta)",
        #"leadjet_pt_4jex":"jet_pt[0]/1000.",
        #"leadjet_pt_5jex":"jet_pt[0]/1000.",
        #"leadjet_pt_6jin":"jet_pt[0]/1000.",
        #"leadjet_pt_4jin":"jet_pt[0]/1000.",
        #"leadjet_pt":"jet_pt[0]/1000.",
        #"leadjet_eta":"abs(jet_eta[0])",
        #"bjet_n_4jin":"Sum$(jet_trueflav==5)",
        #"bjet_n_4jex":"Sum$(jet_trueflav==5)",
        #"bjet_n_5jex":"Sum$(jet_trueflav==5)",
        #"bjet_n_6jin":"Sum$(jet_trueflav==5)",
        ##"HFtype"   :"HFtype",
        #"ht_4jex":"Sum$(jet_pt)/1000.",
        #"ht_5jex":"Sum$(jet_pt)/1000.",
        #"ht_6jin":"Sum$(jet_pt)/1000.",
        #"ht_4jin":"Sum$(jet_pt)/1000.",
        #"ht":"Sum$(jet_pt)/1000.",
        "top_pt_binned":"top_pt/1000.",
        "ttbar_pt_binned":"ttbar_pt/1000.",
        #"top_eta":"abs(top_eta)",
        #"ttbar_eta":"abs(ttbar_eta)",
        #"top_pt":"top_pt/1000.",
        #"ttbar_pt":"ttbar_pt/1000.",
        #"centrality":"centrality",
        #"centrality_4jex":"centrality",
        #"centrality_5jex":"centrality",
        #"centrality_6jin":"centrality",
        #"centrality_4jin":"centrality",
        #"aplanarity":"aplanarity",
        #"aplanarity_4jex":"aplanarity",
        #"aplanarity_5jex":"aplanarity",
        #"aplanarity_6jin":"aplanarity",
        #"aplanarity_4jin":"aplanarity",
}
toppt_bins = array.array("d",[0,50,100,150,200,250,350,800])
ttbarpt_bins = array.array("d",[0,40,170,340,1000])

samples = {
            "PowhegHerwig_rw":("PowhegHerwig","rw_ttbarpt_toppt"),
            "PowhegHerwig":("PowhegHerwig","1"),
            #"PowhegPythiaMiniTruth_full":("PowhegPythiaMiniTruth_full","rw_ttbarpt_toppt"),
            #"MadgraphOff_nominal_rw":("MadgraphOff_nominal","rw_ttbarpt_toppt"),
            #"MadgraphOff_nominal":("MadgraphOff_nominal","1"),
            }

c0 = root.TCanvas("c0")
c1 = root.TCanvas("c1")

for varname, var in vars.iteritems():
  for i,(sample,(file,samplecut)) in enumerate(samples.iteritems()):
    c0.cd()
    rfile = root.TFile.Open("analyze_truth_outfiles/"+file+".root")
    tree  = rfile.Get("particle_jets")
    cut = samplecut
    if not "top_pt" in varname and not "ttbar_pt" in varname:
      cut += "*(lep_n==1)"
    if "Alpgen" or "Madgraph" in sample: weight = "*xsec"
    else: weight = "*xsec"

    if "4j" in varname or "5j" in varname or "6j" in varname:
      hnorm = root.TH1D("hnorm_"+sample,"hnorm_"+sample,20,-0.5,19.5)
      tree.Draw("jet_n >> hnorm_"+sample,cut+weight)

    if   "4jex" in varname: weight += "*(jet_n==4)"
    elif "5jex" in varname: weight += "*(jet_n==5)"
    elif "6jin" in varname: weight += "*(jet_n>=6)"
    elif "4jin" in varname: weight += "*(jet_n>=4)"

    hname = varname+"_"+sample
    print hname
    if "dr"      in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",60,0,6)
    elif "HFtype"  in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",45,-22.5,22.5)
    elif "bjet_n"  in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",10,-0.5,9.5)
    elif "jet_n"   in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",20,-0.5,19.5)
    elif "ttbar_pt_binned" in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",len(ttbarpt_bins)-1,ttbarpt_bins)
    elif "ttbar_pt" in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",50,0,1000)
    elif "top_pt_binned" in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",len(toppt_bins)-1,toppt_bins)
    elif "top_pt" in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",50,0,800)
    elif "sqrt"  in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",50,-10,140)
    elif "pt"    in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",30,0,240)
    elif "ht"    in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",30,0,600)
    elif "eta"   in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",50,0,5)
    elif "phi"   in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",50,-3.14,3.14)
    elif "centrality"   in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",20,0,1)
    elif "aplanarity"   in varname: h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",20,0,0.6)
    else               : h = root.TH1D(hname,sample.replace("over","/")+";"+varname.replace("_"," ")+";events/pb",50,-10,240)

    print cut+weight
    tree.Draw(var+" >> "+hname,cut+weight)
    try:
      if "4j" in varname or "5j" in varname or "6j" in varname:
        h.Scale(1./hnorm.Integral(0,-1))
      else : h.Scale(1./h.Integral(0,-1))
    except ZeroDivisionError:
      print "WTF zero division", hname
      h.SetDirectory(0)
      rfile.Close()
      outfile.cd()
      h.Write()
      continue

    #h.SetDirectory(0)
    if "normalized" in varname:
      h.Scale(1./h.Integral())
    #if "binned" in varname:
    #  nbins = h.GetNbinsX()
    #  for i in range(1,h.GetNbinsX()+1):
    #    error = h.GetBinError(i)
    #    h.SetBinContent(i,h.GetBinContent(i)/h.GetBinWidth(i))
    #    h.SetBinError(i,error)
    if i==4: i=6
    h.SetLineColor(i+1)
    h.SetLineWidth(2)
    if "Mbbover2" in sample or "Sherpa" in sample: h.SetLineStyle(2)

    outfile.cd()
    h.Write()
    rfile.Close()
  #end for samples
#end for vars
outfile.Close()
