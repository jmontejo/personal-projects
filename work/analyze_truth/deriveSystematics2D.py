from optparse import OptionParser
import ROOT as root
import array, os, sys, math

useCategNormDict = False

fine_q1_pt_bins  = array.array("d",[i*80 for i in range(5)]+[400,600,1000])
fine_q1_eta_bins = array.array("d",[i*0.4 for i in range(5)]+[2,2.5])
fine_qq_pt_bins  = array.array("d",[i*80 for i in range(5)]+[400,600,1000])
fine_qq_dr_bins  = array.array("d",[i*0.5 for i in range(4)]+[2,3,6])
coarse_q1_pt_bins  = array.array("d",[i*150 for i in range(4)]+[600,1000])
coarse_q1_eta_bins = array.array("d",[0.5,1,1.5,2.5])
coarse_qq_pt_bins  = array.array("d",[i*150 for i in range(4)]+[600,1000])
coarse_qq_dr_bins  = array.array("d",[i*0.6 for i in range(5)]+[3,6])
ptbins             = array.array("d",[0,50,100,150,200,250,300,400,500,600])
q1_pt_bins = (fine_q1_pt_bins,coarse_q1_pt_bins)
q1_eta_bins = (fine_q1_eta_bins,coarse_q1_eta_bins)
qq_pt_bins = (fine_qq_pt_bins,coarse_qq_pt_bins)
qq_dr_bins = (fine_qq_dr_bins,coarse_qq_dr_bins)

vars = {
        "q1_pt"     : ("q1_pt/1000.",                       (40,0,600), "Leading b-jet p_{T} (GeV)"      ),
        "q1_eta"    : ("abs(q1_eta)",                       (26,0,2.6), "Leading b-jet |#eta|"     ),
        "qq_ht"     : ("qq_ht/1000.",                       (30,0,600), "HT^{bb} (GeV)"       ),
        "qq_pt"     : ("qq_pt/1000.",                       (30,0,600), "p_{T}^{bb} (GeV)"      ),
        "qq_m"      : ("qq_m/1000.",                        (20,0,600), "M^{bb} (GeV)"      ),
        "qq_dr"     : ("qq_dr",                             (30,0,6), "#Delta R^{bb}"        ),
        "ttbar_pt"  : ("ttbar_pt/1000.",                    (len(ptbins)-1,ptbins) , "t#bar{t} p_{T} (GeV)"        ),
        "top_pt"    : ("top_pt/1000.",                      (len(ptbins)-1,ptbins) , "top p_{T} (GeV)"        ),
}

channels = {
        "inc":       "1",
}

categories = {
        #"2gbb": "(convert(HFtype,extHFtype)==22)",
        #"HFbb": "(HFtype > 0)",
        #"realHFbb": ("(extHFtype >= 100)",1,"inclusive",(1,10,20,11,30,2,21,40,3,12,22,31)),
        #"realHFcc": ("(extHFtype < 100)",1,"inclusive",(1,10,20,11,30,2,21,40,3,12,22,31)),
        #"tt1b": "(floor(extHFtype/100) == 1 || floor(extHFtype/100) == 10)",
        #"tt2b": "(floor(extHFtype/100) != 1 && floor(extHFtype/100) != 10)",
        #"ttb" : "(HFtype ==  1)",
        #"ttB" : "(HFtype ==  2)",
        #"ttbb": "(HFtype == 11)",
        #"ttbB": "(HFtype == 12)",
        #"ttBB": "(HFtype == 22)",
        #"ttb": ("(floor(extHFtype/100) == 10)",False,"tt+b",(10,)),
        #"ttB": ("(floor(extHFtype/100) == 1)",False,"tt+B",(1,)),
        "ttbb": ("(floor(extHFtype/100) == 20)",False,"tt+bb",(20,)),
        #"tt3b": ("(floor(extHFtype/100) == 11 || floor(extHFtype/100) == 30 || floor(extHFtype/100) == 2 || floor(extHFtype/100) == 21 || floor(extHFtype/100) == 40 || floor(extHFtype/100) == 3 || floor(extHFtype/100) == 12 || floor(extHFtype/100) == 22 || floor(extHFtype/100) == 31)",True,"tt+3b",(11,30,2,21,40,3,12,22,31) ),
        #"ttc": ("(extHFtype == 10)",False,"tt+c",(10,)),
        #"ttC": ("(extHFtype == 1)",False,"tt+C",(1,)),
        #"ttcc": ("(extHFtype == 20)",False,"tt+cc",(20,)),
        #"tt3c": ("(extHFtype == 11 || extHFtype == 30 || extHFtype == 2 || extHFtype == 21 || extHFtype == 40 || extHFtype == 3 || extHFtype == 12 || extHFtype == 22 || extHFtype == 31)",True,"tt+3c",(11,30,2,21,40,3,12,22,31) ),
}

def main(filename1,filename2,sysname,force,rwvar, rwvar2, prefix=""):
  
  #if rwvar: mode = "read"
  #else    : mode = "update"
  mode = "update"
  outfile = root.TFile.Open("analyze_truth_outfiles/HFsystematics2DSherpa_p15.root",mode)
  rfile1 = root.TFile.Open("analyze_truth_outfiles/"+filename1+".root")
  tree1  = rfile1.Get("particle_jets")
  rfile2 = root.TFile.Open("analyze_truth_outfiles/"+filename2+".root")
  tree2  = rfile2.Get("particle_jets")
  c0  =root.TCanvas("c0")
  canv=root.TCanvas( "canv", "canv",800,800)
  pad_1=root.TPad("pad_1", "up", 0., 0.35, 1., 1.)
  pad_1.SetBottomMargin(0)
  pad_1.Draw()
  pad_2=root.TPad("pad_2", "down", 0.0, 0.00, 1.0, 0.35)
  pad_2.SetTopMargin(0)
  pad_2.SetBottomMargin(0.25)
  pad_2.Draw()

  weight1 = weight2 = "*xsec*rw_ttbarpt_toppt"
  if "Sherpa2" in filename1:
    weight1 = "*xsec*evweight"
  if "Sherpa2" in filename2:
    weight2 = "*xsec*evweight"

  filename1 = filename1.replace("PowhegPythiaMiniTruth","Powheg+Pythia").replace("Sherpa2_","Sherpa+OpenLoops NLO ttbb").replace("MadgraphOff_SL","Madgraph+Pythia").replace("_toppt","").replace("NLO_decayed","")
  filename2 = filename2.replace("PowhegPythiaMiniTruth","Powheg+Pythia").replace("Sherpa2_","Sherpa+OpenLoops NLO ttbb").replace("MadgraphOff_SL","Madgraph+Pythia").replace("_toppt","").replace("NLO_decayed","")
  
  for channame, chan in channels.iteritems():
    for categname, (categ, rebin, categnamepretty,acceptHF) in categories.iteritems():
      isCharm = False
      if "c" in categname: isCharm = True
      cut = chan+"*"+categ
      if len(acceptHF) >=2 and useCategNormDict:
        categNorm, categNormDict = doCategStr(tree1,weight1,tree2,weight2,cut,isCharm)
      else:
        categNorm, categNormDict = "*1",{acceptHF[0]:1}
      #categNorm, categNormDict = "*1",{acceptHF[0]:1}
      
      rwmapname     = "rwmap_"+"_".join([sysname,categname,rwvar,rwvar2])
      rwttbarptname = "rwmap_"+"_".join([sysname,categname,"top_pt","ttbar_pt"])
      if rwttbarptname != rwmapname and rwttbarptname not in outfile.GetListOfKeys():
        print "Run first -v top_pt -V ttbar_pt"
        sys.exit(1)
      rwttbarptmap = outfile.Get(rwttbarptname)
      #if rebin:
      #  bins1 = bins1[1]
      #  bins2 = bins2[1]
      #else:
      #  bins1 = bins1[0]
      #  bins2 = bins2[0]
      #bins = (len(bins1)-1,bins1,len(bins2)-1,bins2)
      if rwmapname not in outfile.GetListOfKeys() or force:
        var1, bins1, fancy1 = vars[rwvar]
        var2, bins2, fancy2 = vars[rwvar2]
        bins = bins1+bins2
        print "Not precomputed:",rwmapname
        sys.exit(1)
        rwmap  = root.TH2F(rwmapname,rwmapname,*bins)
        rwmap2 = root.TH2F(rwmapname+"2",rwmapname+"2",*bins)
        c0.cd()
        tree1.Draw(var2+":"+var1+" >> "+rwmapname     ,cut+weight1)
        if rwttbarptname == rwmapname:
          tree2.Draw(var2+":"+var1+" >> "+rwmapname+"2" ,cut+weight2+categNorm)
        else:
          #----------------------
          for i in xrange(tree2.GetEntries()):
            tree2.GetEntry(i)
            if not isCharm and not math.floor(tree2.extHFtype/100) in acceptHF: continue
            if     isCharm and not tree2.extHFtype in acceptHF: continue
            therwvar = getattr(tree2,rwvar)
            therwvar2 = getattr(tree2,rwvar2)
            weight = tree2.rw_ttbarpt_toppt*tree2.xsec
            if "eta" in rwvar: 
              therwvar = abs(therwvar)
            elif not "dr" in rwvar: 
              therwvar = therwvar/1000.
            if "eta" in rwvar2: 
              therwvar2 = abs(therwvar2)
            elif not "dr" in rwvar2: 
              therwvar2 = therwvar2/1000.
            weight *= rwttbarptmap.GetBinContent(rwttbarptmap.FindBin(getattr(tree2,"top_pt")/1000.,getattr(tree2,"ttbar_pt")/1000.))
      
            if useCategNormDict:
              if isCharm:
                extHFtype = tree2.extHFtype
              else:
                extHFtype = math.floor(tree2.extHFtype/100)
              if extHFtype != 1 and extHFtype != 10 and extHFtype != 20:
                extHFtype = 21
              weight *= categNormDict[extHFtype]

            rwmap2.Fill(therwvar,therwvar2,weight)
          #----------------------
        rwmap.Scale(1./rwmap.Integral(0,-1,0,-1))
        rwmap2.Scale(1./rwmap2.Integral(0,-1,0,-1))
        rwmap.Divide(rwmap2)
        for binx in range(1,rwmap.GetNbinsX()+1):
          for biny in range(1,rwmap.GetNbinsY()+1):
            if rwmap.GetBinContent(binx,biny) ==0:
              rwmap.SetBinContent(binx,biny,1)
        outfile.cd()
        rwmap.Write()
        continue
      else:
        rwmap = outfile.Get(rwmapname)
      for varname, (var, binning, varnamepretty) in vars.iteritems():
        #if doSkip(channame, categname, varname, rwvar): 
        #  print "skipping:",channame,categname, varname, rwvar
        #  continue
        #if rwvar == varname or rwvar2 == varname: continue
  
        hname = "_".join([sysname,categname,varname])
        if not force and "hist_"+hname in outfile.GetListOfKeys():
          print "Already in:",hname
          continue
        print hname
        if "extHFtype" in varname: ylabel = "events/pb"
        else:                      ylabel = "arbitrary units"
        h1  = root.TH1F(hname     ,hname+";"+varnamepretty+";"+ylabel,*binning)
        h2  = root.TH1F(hname+"2" ,hname+";"+varnamepretty+";"+ylabel,*binning)
        
        c0.cd()
        print(var+" >> "+hname     ,cut+weight1)
        tree1.Draw(var+" >> "+hname     ,cut+weight1)
        tree2.Draw(var+" >> "+hname+"2" ,cut+weight2+categNorm)
        hrwname = hname+"_rw_"+rwvar2+"_rw_"+rwvar
        hrwnamepretty = hname+" NLO rw."
        hrw = root.TH1F(hrwname,hrwnamepretty+";"+varnamepretty+";"+ylabel,*binning)
#----------------------------
        #rw = doRwStr(rwmap, bins, vars[rwvar][0],vars[rwvar2][0])
        #tree2.Draw(var+" >> "+hrwname,cut+weight2+categNorm+rw)
        for i in xrange(tree2.GetEntries()):
          tree2.GetEntry(i)
          if not isCharm and not math.floor(tree2.extHFtype/100) in acceptHF: continue
          if     isCharm and not tree2.extHFtype in acceptHF: continue
          thevar = getattr(tree2,varname)
          therwvar = getattr(tree2,rwvar)
          therwvar2 = getattr(tree2,rwvar2)
          if "eta" in varname: 
            thevar = abs(thevar)
          elif not "dr" in varname: 
            thevar = thevar/1000.
          if "eta" in rwvar: 
            therwvar = abs(therwvar)
          elif not "dr" in rwvar: 
            therwvar = therwvar/1000.
          if "eta" in rwvar2: 
            therwvar2 = abs(therwvar2)
          elif not "dr" in rwvar2: 
            therwvar2 = therwvar2/1000.
          weight = tree2.rw_ttbarpt_toppt*tree2.xsec*rwmap.GetBinContent(rwmap.FindBin(therwvar,therwvar2))
          if rwttbarptname != rwmapname:
            weight *= rwttbarptmap.GetBinContent(rwttbarptmap.FindBin(getattr(tree2,"top_pt")/1000.,getattr(tree2,"ttbar_pt")/1000.))
      
          if useCategNormDict:
            if isCharm:
              extHFtype = tree2.extHFtype
            else:
              extHFtype = math.floor(tree2.extHFtype/100)
            if extHFtype != 1 and extHFtype != 10 and extHFtype != 20:
              extHFtype = 21
            weight *= categNormDict[extHFtype]

          hrw.Fill(thevar,weight)
          #print "Maan",(categNormDict[math.floor(tree2.extHFtype/100)],thevar,tree2.xsec,rwmap.GetBinContent(rwmap.FindBin(therwvar,therwvar2))/0.543,rwmap.FindBin(therwvar,therwvar2),therwvar,therwvar2)
          #if rwmap.GetBinContent(rwmap.FindBin(therwvar,therwvar2))>=2:
          #  print thevar,therwvar,therwvar2, rwmap.GetBinContent(rwmap.FindBin(therwvar,therwvar2)),rwmap.FindBin(therwvar,therwvar2)
          #  sys.exit(1)
#----------------------------
        print h2.GetEntries(),hrw.GetEntries()
        if h1.Integral() == 0 or h2.Integral() == 0:
          print "empty, consider skipping:",varname, channame, categname, h1.Integral(), h2.Integral()
          if not rwvar:
            for bin in range(1,h1.GetNbinsX()+1):
              h1.SetBinContent(bin,1)
            outfile.cd()
            h1.Write("",root.TObject.kOverwrite)
          continue
  
        if "extHFtype" in varname:
          ref = h2.Clone("ref")
          h1 = prettyHF(h1,ref)
          h2 = prettyHF(h2,ref)
          if rwvar: hrw = prettyHF(hrw,ref)
        #------------------- plot and store
        h1.SetLineWidth(2)
        h2.SetLineWidth(2)
        h2.SetLineColor(2)
        if rwvar:
          print h1.Integral(0,-1), hrw.Integral(0,-1)
          hrw.SetLineWidth(2)
          hrw.SetLineColor(2)
          hrw.SetLineStyle(2)
        pad_1.cd()
        h1.Scale(1./h1.Integral(0,-1))
        h2.Scale(1./h2.Integral(0,-1))
        print h1.Integral(0,-1), h2.Integral(0,-1)
        h1.SetMaximum(max(h1.GetMaximum(),h2.GetMaximum())*1.2)
        if not "dr" in varname and not "eta" in varname: 
          h1.SetMaximum(h1.GetMaximum()*2)
        if "extHFtype" in varname:
          h1.SetMaximum(h1.GetMaximum()*100)
        h1.DrawCopy()
        h2.DrawCopy("same")
        leg = root.TLegend(0.45,0.55,0.89,0.89)
        leg.SetHeader(", ".join([categnamepretty,varnamepretty]))
        leg.SetFillStyle(0)
        leg.SetLineColor(0)
        nicename1 = filename1.replace("Off_"," ").replace("MiniTruth","").replace("Fix","").replace("_HF","").replace("up"," up").replace("down"," down")
        nicename2 = filename2.replace("Off_"," ").replace("MiniTruth","").replace("Fix","").replace("_HF","").replace("up"," up").replace("down"," down")
        nicename1 = nicename1.replace("nonprompt","").replace("Powheg","Powheg+").replace("MPI","")
        nicename2 = nicename2.replace("nonprompt","").replace("Powheg","Powheg+").replace("MPI","")
        leg.AddEntry(h1,nicename1)
        leg.AddEntry(h2,nicename2)
        if rwvar:
          hrw.Scale(1./hrw.Integral(0,-1))
          hrw.DrawCopy("same")
          leg.AddEntry(hrw,nicename2+" reweighted")
        leg.Draw()
        pad_2.cd()
        if rwvar:
          h1c = h1.Clone(hrwname)
        #print varname,": chi2, kolmogorov =",h1.Chi2Test(hrw,"WW"),h1.KolmogorovTest(hrw)
        outfile.cd()
        h1.Write("hist_"+h1.GetName(),root.TObject.kOverwrite)
        h2.Write("hist_"+h2.GetName(),root.TObject.kOverwrite)
        hrw.Write("hist_"+hrw.GetName(),root.TObject.kOverwrite)
        h1.Divide(h2)
        h1.SetMaximum(1.99)
        h1.SetMinimum(0.01)
        h1.GetYaxis().SetTitle("")
        h1.GetXaxis().SetLabelSize(0.06)
        h1.GetYaxis().SetLabelSize(0.06)
        h1.GetXaxis().SetTitleOffset(1.2)
        h1.GetXaxis().SetTitleSize(0.06)
        h1.DrawCopy()
        if rwvar:
          h1c.Divide(hrw)
          h1c.SetLineStyle(2)
          h1c.DrawCopy("same")
        line = root.TLine(h1.GetBinLowEdge(1),1,h1.GetBinLowEdge(h1.GetNbinsX()+1),1)
        line.Draw()
        if rwvar:
          if rwvar2: hname += "_rw_"+rwvar2
          hname += "_rw_"+rwvar
        print "printing",hname
        if not "dr" in varname and not "eta" in varname: 
          pad_1.SetLogy(1);
        else:
          pad_1.SetLogy(0);
        #canv.SaveAs("plots_deriveHFsys/"+hname+".pdf")
        canv.SaveAs("plots_deriveHFsys/"+prefix+hname.replace("ttB","tt1gbbq").replace("ttb_","tt1bq_")+".eps")
        #canv.SaveAs("plots_deriveHFsys/"+hname+".C")
        outfile.cd()
        if rwvar:
          h1c.Write("",root.TObject.kOverwrite)
        h1.Write("",root.TObject.kOverwrite)
        
        #------------------- plot and store


      #end var
    #end categ
  #end chan
  outfile.Close()
  rfile1.Close()
  rfile2.Close()
#-----------------------

skipChanCat = {
      "1addjetex":("tt",),
      "2addjet"  :("tt",),
}
skipChanVar = {
      "1addjetex":("qq","q2"),
}
skipCatVar = {
      "ttb":("qq","q2"),
      "ttB":("qq","q2"),
      "tt1b":("qq","q2"),
      "tt1bq":("qq","q2"),
      "tt1Bq":("qq","q2"),
}

def doSkip(channame, categname, varname, rwvar):
  
  for chan, cats in skipChanCat.iteritems():
    if chan in channame:
      for cat in cats:
        if cat in categname: return True
  for chan, vars in skipChanVar.iteritems():
    if chan in channame:
      for var in vars:
        if var in varname: return True
  for cat, vars in skipCatVar.iteritems():
    if cat == categname:
      for var in vars:
        if var in varname: return True
  for chan, vars in skipChanVar.iteritems():
    if chan in channame:
      for var in vars:
        if var in rwvar: return True
  for cat, vars in skipCatVar.iteritems():
    if cat in categname:
      for var in vars:
        if var in rwvar: return True
  return False

def doCategStr(tree1, weight1, tree2, weight2,cut,isCharm):
  ctgstr = "*("
  if isCharm:
    var,bins = ("extHFtype",         (50,0.5,50.5)   )
  else:
    var,bins = ("floor(extHFtype/100)",         (50,0.5,50.5)   )
  h1 = root.TH1F("h1","h1",*bins)
  h2 = root.TH1F("h2","h2",*bins)
  c01  =root.TCanvas("c01")
  c01.cd()
  tree1.Draw(var+" >> h1",cut+weight1)
  tree2.Draw(var+" >> h2",cut+weight2)
  multib = 0
  for bin in range(1,h1.GetNbinsX()+1):
    if h1.GetBinCenter(bin) != 1 and h1.GetBinCenter(bin) != 10  and h1.GetBinCenter(bin) != 20:
      multib += h1.GetBinContent(bin)
      h1.SetBinContent(bin,0)
  h1.SetBinContent(21,multib)
  multib = 0
  for bin in range(1,h2.GetNbinsX()+1):
    if h2.GetBinCenter(bin) != 1 and h2.GetBinCenter(bin) != 10  and h2.GetBinCenter(bin) != 20:
      multib += h2.GetBinContent(bin)
      h2.SetBinContent(bin,0)
  h2.SetBinContent(21,multib)
  h1.Scale(1./h1.Integral())
  h2.Scale(1./h2.Integral())
  h1.Divide(h2)
  theDict = {}
  for bin in range(1,h1.GetNbinsX()+1):
    if h1.GetBinContent(bin) == 0: continue
    print "%d %f" % (int(h1.GetBinLowEdge(bin)+0.5),h1.GetBinContent(bin))
    sbin = "(%s >= %s && %s < %s)*%s+" % (var,str(h1.GetBinLowEdge(bin)),var,str(h1.GetBinLowEdge(bin+1)),str(h1.GetBinContent(bin)))
    theDict[int(h1.GetBinLowEdge(bin)+0.5)]=h1.GetBinContent(bin)
    ctgstr += sbin
  ctgstr = ctgstr[:-1]+")"
  del h1, h2
  return ctgstr,theDict

def doRwStr(h,bins, rwvar,rwvar2):
  nx, lowx, highx, ny, lowy, highy = bins
  rwstr = "*("
  for binx in range(nx):
    for biny in range(ny):
      print str(lowx+binx*highx/nx) ,str(lowy+biny*highy/ny),str(h.GetBinContent(binx+1, biny+1))
      if h.GetBinContent(binx+1, biny+1) == 0: continue
      sbin = "(%s >= %s && %s < %s && %s >= %s && %s < %s)*%s+" % (rwvar,str(lowx+binx*highx/nx),rwvar,str(lowx+(binx+1)*highx/nx),rwvar2,str(lowy+biny*highy/ny),rwvar2,str(lowy+(biny+1)*highy/ny),str(h.GetBinContent(binx+1, biny+1)))
      rwstr += sbin
  rwstr = rwstr[:-1]+")"
  print "doRwStr:",rwstr
  sys.exit(1)
  return rwstr

def prettyHF(original, ref=None):

  bins = []
  if not ref: ref = original
  for bin in xrange(1,ref.GetNbinsX()+1):
    cont = ref.GetBinContent(bin)
    hf   = int(ref.GetBinCenter(bin))
    if not cont: continue
    label = "tt"
    if hf == 0: label = "tt+jets"
    if hf > 0:  label = "tt"+"b"*(hf/10)+"B"*(hf%10)
    if hf < 0:  label = "tt"+"c"*(hf/10)+"C"*(hf%10)
    bins.append((bin, label))
    
  new = root.TH1F(original.GetName(),original.GetTitle(),len(bins),0,len(bins))
  for i, (bin, label) in enumerate( bins):
    new.SetBinContent(i+1,original.GetBinContent(bin))
    new.SetBinError(i+1,original.GetBinError(bin))
    new.GetXaxis().SetBinLabel(i+1, label)
  new.GetXaxis().LabelsOption("v")
  return new

if __name__ == "__main__":

  parser = OptionParser() 
  parser.add_option("-f", "--force", 
                    dest="force", 
                    help="Overwrite existing systematic", 
                    action="store_true", 
                    default=False)
  parser.add_option("-s", "--sysname", 
                    dest="sysname", 
                    help="Name to be givent to the systematic", 
                    default="defaultsys")
  parser.add_option("-v", "--varname", 
                    dest="varname", 
                    help="Apply a reweighting based on this variable", 
                    default="")
  parser.add_option("-V", "--varname2", 
                    dest="varname2", 
                    help="Apply a reweighting based on this variable", 
                    default="")
  parser.add_option("-p", "--prefix", 
                    dest="prefix", 
                    help="Prefix for the plots",
                    default="")
  parser.add_option("-a",
                    dest="allvar", 
                    help="Run and reweight all variables",
                    action="store_true", 
                    default="")

  options,args = parser.parse_args() 
  if len(args) != 2:
    print "Need to specify the two files to compare"
    sys.exit(1)

  file1 = "analyze_truth_outfiles/"+args[0]+".root"
  file2 = "analyze_truth_outfiles/"+args[1]+".root"
  if not os.path.exists(file1):
    print "Does not exist:",file1
    sys.exit(1)
  if not os.path.exists(file2):
    print "Does not exist:",file2
    sys.exit(1)

  root.gROOT.SetBatch(1)
  root.TH1.SetDefaultSumw2(1)
  root.gStyle.SetOptTitle(0)
  root.gStyle.SetOptStat(0)
  #root.gROOT.ProcessLineSync(".x convert.C+")

  if options.allvar:
    main(args[0],args[1],options.sysname,options.force,"","")
    for var in vars:
      main(args[0],args[1],options.sysname,options.force,var,"")
  else:
    main(args[0],args[1],options.sysname,options.force,options.varname, options.varname2,options.prefix)
