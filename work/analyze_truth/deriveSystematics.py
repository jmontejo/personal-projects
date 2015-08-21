from optparse import OptionParser
import ROOT as root
import array, os, sys

vars = {
        #"add_bjet_n": ("Sum$(jet_trueflav==5 && jet_id!=1)",(4,0.5,4.5)     ),
        #"extHFtype"    : ("floor(extHFtype/100)",         (50,0.5,50.5), "HF type"   ),
        #"extHFtype_ttcc"    : ("extHFtype",         (50,0.5,50.5), "HF type"   ),
        #"HFtype"    : ("(floor(HFtype/10)*2+HFtype%10)",         (13,-6.5,6.5)   ),
        "q1_pt"     : ("q1_pt/1000.",                       (30,0,600), "leading b pT"      ),
        #"q1_eta"    : ("abs(q1_eta)",                       (30,0,3.0), "leading b eta"     ),
        #"q2_pt"     : ("q2_pt/1000.",                       (30,0,300), "subleading b pT"       ),
        #"qq_ht"     : ("qq_ht/1000.",                       (30,0,600), "HT_{bb}"       ),
        #"qq_pt"     : ("qq_pt/1000.",                       (30,0,600), "pT_{bb}"      ),
        #"qq_m"      : ("qq_m/1000.",                        (30,0,600), "M_{bb}"      ),
        #"qq_dr"     : ("qq_dr",                             (30,0,6), "#Delta R_{bb}"        ),
        #"top_pt"     : ("top_pt/1000.",                      (20,0,800), "top pT"        ),
        #"ttbar_pt"     : ("ttbar_pt/1000.",                      (20,0,800), "ttbar pT"        ),
}

channels = {
        #"5jetex": "(jet_n==5)",
        #"6jetin": "(jet_n>=6)",
        #"2addjetex": "( Sum$(jet_trueflav==5 && jet_id!=1) == 2 )",
        #"1addjetex": "( Sum$(jet_trueflav==5 && jet_id!=1) == 1 )",
        #"1addjetin": "( extHFtype>=100 )"
        #"2addjetin": "( Sum$(jet_trueflav==5 && jet_id!=1) >= 2 )",
        "inc":       "1",
}

categories = {
        #"inc": ("1",1,"inclusive"),
        #"realHFbb": ("(extHFtype>=100)",1,"inclusive"),
        #"realHFbb_ljets": ("(extHFtype>=100 && lep_n==2)",1,"inclusive"),
        #"realHFbb_dilep": ("(extHFtype>=100 && lep_n==1)",1,"inclusive"),
        "tt1bq": ("(floor(extHFtype/100) == 10)",1,"tt+b"),
        "tt1gbbq": ("(floor(extHFtype/100) == 1)",1,"tt+B"),
        "tt2bq": ("(floor(extHFtype/100) == 20)",1,"tt+bb"),
        #"higgswindow": ("(abs(qq_m/1000.-125)<30)",1,"Higgs mass window (30 GeV)"),
        #"tt3bq": ("(floor(extHFtype/100) == 11 || floor(extHFtype/100) == 30)",2,"tt+3b"),
        #"tt4bq": ("(floor(extHFtype/100) == 2 || floor(extHFtype/100) == 21 || floor(extHFtype/100) == 40 || floor(extHFtype/100) == 3 || floor(extHFtype/100) == 12 || floor(extHFtype/100) == 22 || floor(extHFtype/100) == 31)",3,"tt+4b" ),
        #"realHFcc": ("(extHFtype<100)",1,"inclusive"),
        #"tt1cq": ("(extHFtype == 10)",1,"tt+c"),
        #"tt1gccq": ("(extHFtype == 1)",1,"tt+C"),
        #"tt2cq": ("(extHFtype == 20)",1,"tt+cc"),
        #"tt3cq": ("(extHFtype == 11 || extHFtype == 30)",2,"tt+3c"),
        #"tt4cq": ("(extHFtype == 2 || extHFtype == 21 || extHFtype == 40 || extHFtype == 3 || extHFtype == 12 || extHFtype == 22 || extHFtype == 31)",3,"tt+4c" ),
}

def main(filename1,filename2,sysname,force,rwvar, seqrwvar):
  
  #if rwvar: mode = "read"
  #else    : mode = "update"
  mode = "read"
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

  weight1 = weight2 = "*xsec*rw_ttbarpt_toppt/0.543"
  #weight1 = "*xsec/0.543"
  if "Sherpa2" in filename1:
    weight1 = "*xsec*evweight"
  if "Sherpa2" in filename2:
    weight2 = "*xsec*evweight"
  if "MadgraphOff_SL" in filename2:
    weight2 = "*252.89/14910000"
  if "MadgraphOff_SL" in filename1:
    weight1 = "*((ttbar_pt/1000. >= 0.0 && ttbar_pt/1000. < 40.0)*0.978779673576+(ttbar_pt/1000. >= 40.0 && ttbar_pt/1000. < 170.0)*1.02676892281+(ttbar_pt/1000. >= 170.0 && ttbar_pt/1000. < 340.0)*1.01000106335+(ttbar_pt/1000. >= 340.0 && ttbar_pt/1000. < 1000.0)*1.00781619549)*((top_pt/1000. >= 0.0 && top_pt/1000. < 50.0)*1.0093331337+(top_pt/1000. >= 50.0 && top_pt/1000. < 100.0)*1.01089119911+(top_pt/1000. >= 100.0 && top_pt/1000. < 150.0)*1.01698601246+(top_pt/1000. >= 150.0 && top_pt/1000. < 200.0)*1.01620137691+(top_pt/1000. >= 200.0 && top_pt/1000. < 250.0)*0.954322934151+(top_pt/1000. >= 250.0 && top_pt/1000. < 350.0)*0.87033188343+(top_pt/1000. >= 350.0 && top_pt/1000. < 800.0)*0.780595779419)*252.89/14910000"
  if "_Had" in filename2:
    weight2 += "*1.4"
  if "dec_hadr" in filename1:
    weight1 += "/(0.011664)"
#  if "Herwig" in filename1:
#    weight1 += "*1.28"
  print weight1, weight2

  #filename1 = filename1.replace("PowhegPythiaMiniTruth","Powheg+Pythia").replace("Sherpa2_","Sherpa+OpenLoops NLO ttbb: ").replace("MadgraphOff_SL","Madgraph+Pythia").replace("_toppt","")
  #filename2 = filename2.replace("PowhegPythiaMiniTruth","Powheg+Pythia").replace("Sherpa2_","Sherpa+OpenLoops NLO ttbb: ").replace("MadgraphOff_SL","Madgraph+Pythia").replace("_toppt","")
  #filename1 = "Powheg+Herwig"
  #filename1 = "Powheg+Pythia, not reweighted"
  #filename2 = "Powheg+Pythia, reweighted"
  filename1 = "Sherpa ttbb LOPS, decayed and hadronized"
  filename2 = "Sherpa ttbb LOPS, not decayed, not hadronized"
  
  for channame, chan in channels.iteritems():
    for categname, (categ, rebin, categnamepretty) in categories.iteritems():
      cut = chan+"*"+categ
      isCharm =False
      if "c" in categname: isCharm = True
      #categNorm = "*1"
      if len(vars) > 2 or (not "extHFtype" in vars.keys() and not "extHFtype_ttcc" in vars.keys()):
        categNorm = doCategStr(tree1,weight1,tree2,weight2,cut,isCharm)
      else:
        categNorm = "*1"
      for varname, (var, binning, varnamepretty) in vars.iteritems():
        if not "HFtype" in varname:
          binning = (binning[0]/rebin,binning[1],binning[2])
        if doSkip(channame, categname, varname, rwvar): 
          print "skipping:",channame,categname, varname, rwvar
          continue
        if rwvar and rwvar == varname and not seqrwvar: continue
        #if rwvar and rwvar == varname: continue
        #if seqrwvar and seqrwvar == varname: continue
  
        hname = "_".join([sysname,categname,varname])
        if seqrwvar:
          hrwname = "_".join([sysname,categname,rwvar,"rw",seqrwvar])
          if hrwname not in outfile.GetListOfKeys():
            print "Not precomputed:",hrwname
            break
          hratio = outfile.Get(hrwname)
          hseqrwname = "_".join([sysname,categname,seqrwvar])
          if hseqrwname not in outfile.GetListOfKeys():
            print "Not precomputed:",hseqrwname
            break
          hseqratio = outfile.Get(hseqrwname)
        elif rwvar:
          hrwname = "_".join([sysname,categname,rwvar])
          if hrwname not in outfile.GetListOfKeys():
            print "Not precomputed:",hrwname
            break
          hratio = outfile.Get(hrwname)
        print hname
        if "extHFtype" in varname: ylabel = "events/pb"
        else:                      ylabel = "arbitrary units"
        h1  = root.TH1F(hname     ,hname+";"+varnamepretty+";"+ylabel,*binning)
        h2  = root.TH1F(hname+"2" ,hname+";"+varnamepretty+";"+ylabel,*binning)
        
        c0.cd()
        print "tree1",(var+" >> "+hname     ,cut+weight1)
        tree1.Draw(var+" >> "+hname     ,cut+weight1)
        print "tree2",(var+" >> "+hname     ,cut+weight2+categNorm)
        tree2.Draw(var+" >> "+hname+"2" ,cut+weight2+categNorm)
        if rwvar:
          if seqrwvar:
            hrwname = hname+"_rw_"+seqrwvar+"_rw_"+rwvar
          else:
            hrwname = hname+"_rw_"+rwvar
          hrw = root.TH1F(hrwname,hrwname+";"+varnamepretty+";"+ylabel,*binning)
          rw = doRwStr(hratio,vars[rwvar][0])
          if seqrwvar:
            seqrw = doRwStr(hseqratio,vars[seqrwvar][0])
            tree2.Draw(var+" >> "+hrwname,cut+weight2+categNorm+rw+seqrw)
          else:
            tree2.Draw(var+" >> "+hrwname,cut+weight2+categNorm+rw)
        if h1.Integral() == 0 or h2.Integral() == 0:
          print "empty, consider skipping:",varname, channame, categname, h1.Integral(), h2.Integral()
          continue
  
        if "extHFtype" in varname:
          h2copy = h2.Clone("copy")
          h2 = prettyHF(h2,h1,isCharm)
          h1 = prettyHF(h1,h2copy,isCharm)
          if rwvar: hrw = prettyHF(hrw,h2copy,isCharm)
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
        #h1.Scale(1./h1.Integral(0,-1))
        #h2.Scale(1./h2.Integral(0,-1))
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
        leg.AddEntry(h1,filename1)
        leg.AddEntry(h2,filename2)
        if rwvar:
          #hrw.Scale(1./hrw.Integral(0,-1))
          hrw.DrawCopy("same")
          if seqrwvar:
            leg.AddEntry(hrw,filename2+" rw "+seqrwvar+" rw "+rwvar)
          else:
            leg.AddEntry(hrw,filename2+" rw "+rwvar)
        leg.Draw()
        pad_2.cd()
        if rwvar:
          h1c = h1.Clone(hrwname)
        h1.Divide(h2)
        h1.SetMaximum(1.99)
        h1.SetMinimum(0.01)
        h1.GetYaxis().SetTitle("")
        if "extHFtype" in varname:
          h1.GetXaxis().SetLabelSize(0.12)
        else:
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
          if seqrwvar: hname += "_rw_"+seqrwvar
          hname += "_rw_"+rwvar
        print "printing",hname
        if not "dr" in varname and not "eta" in varname: 
          pad_1.SetLogy(1);
        else:
          pad_1.SetLogy(0);
        #canv.SaveAs("plots_deriveHFsys/"+hname+".pdf")
        canv.SaveAs("plots_deriveHFsys/"+hname+".png")
        
        #------------------- plot and store

      #end var
    #end categ
  #end chan
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
      "tt1gbbq":("qq","q2"),
      "ttc":("qq","q2"),
      "ttC":("qq","q2"),
      "tt1c":("qq","q2"),
      "tt1cq":("qq","q2"),
      "tt1gccq":("qq","q2"),
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
  print (var+" >> h1",cut+weight1),(var+" >> h2",cut+weight2), h1.Integral(), h2.Integral()
  h1.Divide(h2)
  for bin in range(1,h1.GetNbinsX()+1):
    if h1.GetBinContent(bin) == 0: continue
    print "%d %f" % (int(h1.GetBinLowEdge(bin)+0.5),h1.GetBinContent(bin))
    sbin = "(%s >= %s && %s < %s)*%s+" % (var,str(h1.GetBinLowEdge(bin)),var,str(h1.GetBinLowEdge(bin+1)),str(h1.GetBinContent(bin)))
    ctgstr += sbin
  ctgstr = ctgstr[:-1]+")"
  del h1, h2
  print ctgstr
  return ctgstr

def doRwStr(h,rwvar):
  rwstr = "*("
  for bin in range(1,h.GetNbinsX()+1):
    if h.GetBinContent(bin) == 0: continue
    sbin = "(%s >= %s && %s < %s)*%s+" % (rwvar,str(h.GetBinLowEdge(bin)),rwvar,str(h.GetBinLowEdge(bin+1)),str(h.GetBinContent(bin)))
    rwstr += sbin
  rwstr = rwstr[:-1]+")"
  print rwstr
  return rwstr

def prettyHF(original, other, isCharm):

  bins = []
  intoriginal = original.Integral()
  intother = other.Integral()
  for bin in xrange(1,original.GetNbinsX()+1):
    cont1 = original.GetBinContent(bin)
    cont2 = other.GetBinContent(bin)
    if not cont1 or not cont2: continue
    if cont1/intoriginal < 1e-4 or cont2/intother < 1e-4: continue
    print bin, cont1, cont2
    hf   = int(other.GetBinCenter(bin))
    if hf >= 50: continue
    label = "tt"
    if hf == 0: label = "tt+jets"
    if isCharm: label = "tt"+"c"*(hf/10)+"C"*(hf%10)
    else:       label = "tt"+"b"*(hf/10)+"B"*(hf%10)
    #if hf > 0:  label = "tt"+"b"*(hf/10)+"B"*(hf%10)
    #if hf < 0:  label = "tt"+"c"*(hf/10)+"C"*(hf%10)
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
  parser.add_option("-V", "--seqvarname", 
                    dest="seqvarname", 
                    help="Apply the reweighting after an already reweighted based on this variable", 
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
    main(args[0],args[1],options.sysname,options.force,options.varname, options.seqvarname)
