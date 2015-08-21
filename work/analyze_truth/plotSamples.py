from optparse import OptionParser
import ROOT as root
import array, os, sys
import operator
import importlib

#etabins = array.array("d",[x*0.2 for x in range(12)]+[2.5])
#ptbins = array.array("d",[0,40,80,120,160,200,250,300,400,500,600])

colorMap = {
    0:{
        0:(1,1),
        1:(2,1),
        2:(4,1),
    },
    1:{
        0:(1,1),
        1:(2,1),
        2:(2,2),
    },
    2:{
        0:(1,1),
        1:(2,1),
        2:(4,1),
        3:(3,1),
        4:(6,1),
        5:(7,1),
    }
}

def include(filename):
    if os.path.exists(filename): 
        execfile(filename)

channels = {"inc": "1"}

def main(names,prefix,reference,band,norm,store,color,noplotmpi,zoomratio):
  
  c0  =root.TCanvas("c0")
  canv=root.TCanvas( "canv", "canv",800,800)
  pad_1=root.TPad("pad_1", "up", 0., 0.35, 1., 1.)
  pad_1.SetBottomMargin(0)
  pad_1.Draw()
  pad_2=root.TPad("pad_2", "down", 0.0, 0.00, 1.0, 0.35)
  pad_2.SetTopMargin(0)
  pad_2.SetBottomMargin(0.30)
  pad_2.Draw()
  if store:
    outfile = root.TFile.Open("analyze_truth_outfiles/plots.root","update")

  files = {}
  for name in names:
    file = "analyze_truth_outfiles/"+name.replace("_IFSR","").replace("_MCgen","").replace("_up","").replace("_down","").replace("_lepjets","").replace("_dilepton","").replace("_dimuon","").replace("_singlemuon","").replace("_rwHF","").replace("_norw","")+".root"
    file = file.replace("_ME0b","").replace("_ME1b","").replace("_ME2b","")
    if not os.path.exists(file):
      print "Does not exist:",file
      sys.exit(1)

    weight = "*xsec*rw_ttbarpt_toppt/0.543"
    if "dimuon" in name:
      weight = "*xsec*rw_ttbarpt_toppt*(muon_n==2)*9/(0.105)"
    if "dilepton" in name:
      weight = "*xsec*rw_ttbarpt_toppt*(lep_n==2)/(0.105)"
    if "lepjets" in name:
      weight = "*xsec*rw_ttbarpt_toppt*(lep_n==1)/(0.438)"
    if "singlemuon" in name:
      weight = "*xsec*rw_ttbarpt_toppt*(lep_n==1 && muon_n==1)*3/(0.438)"
    #weight = "*xsec*rw_ttbarpt_toppt*(lep_n==2)/(0.105*1.1992)" #pdg BR
    if "Sherpa2" in name and "SemiLep" in name:
      weight = "*1.2*xsec*evweight/0.07409"
    elif "Sherpa2" in name and "stable" in name:
      weight = "*1.2*xsec*evweight"
    elif "Sherpa2" in name:
      weight = "*1.2*xsec*evweight/0.01236"
    if "_Had" in name:
      weight += "*1.4"
    if "MCgen"  in name or "IFSR" in name:
      weight += getSyst(name)
    if "ME0b" in name:
      weight += "*(num_ME_b==0)"
    if "ME1b" in name:
      weight += "*(num_ME_b==1)"
    if "ME2b" in name:
      weight += "*(num_ME_b>=2)"
    if "norw" in name:
      weight = weight.replace("*rw_ttbarpt_toppt","")
    if "rwHF" in name:
      weight += "*rw_HF"

    files[name] = [file,weight,"",None]
  if not reference: reference = names[0]
  keys = [reference]
  for name in files.keys():
    if name not in keys:
      keys.append(name)
  keys = names

  for channame, chan in channels.iteritems():
    for categname, (categ, rebin, categnamepretty) in jo.categories.iteritems():
      cut = chan+"*"+categ
      isCharm =False
      if "c" in categname: isCharm = True
      isMEsplit = all(["withMEb" in x for x in files.iterkeys()])
      for key in files.iterkeys():
        if False and key!=reference and not all(["HFtype" in x for x in jo.vars.keys()]):
          categNorm = doCategStr(files,key,reference,cut,isCharm)
        else:
          categNorm = "*1"
        files[key][2] = categNorm

    
      for varname, (var, binning, varnamepretty) in jo.vars.iteritems():
        if not "HFtype" in varname and not "jet_n" in varname and len(binning)==3:
          binning = (binning[0]/rebin,binning[1],binning[2])
        if doSkip(channame, categname, varname): 
          print "skipping:",channame,categname, varname
          continue
  
        if "extHFtype" in varname: ylabel = "events/pb"
        else:                      ylabel = "Arbitrary units"

        tex = root.TLatex()
        tex.SetTextSize(0.06)
        tex.SetTextFont(42)
        tex.SetTextAlign(12)
        leg = root.TLegend(0.48-0.04*(isMEsplit),0.61,0.89,0.88)
        leg.SetFillStyle(0)
        leg.SetLineColor(0)
        if band:
          bandh = None

        for c,name in enumerate(keys):
          file,weight,categNorm,h = files[name]
          rfile  = root.TFile.Open(file)
          tree   = rfile.Get("particle_jets")

          hname = "_".join([prefix,categname,varname,name])
          print hname

          c0.cd()
          h  = root.TH1D(hname     ,hname+";"+varnamepretty+";"+ylabel,*binning)
          tree.Draw(var+" >> "+hname     ,cut+weight+categNorm)
          #print (var+" >> "+hname     ,cut,weight,categNorm)
          if h.Integral() == 0:
            print "empty, consider skipping:",hname,  h.Integral(0,-1)
            continue
  
          if "extHFtype" in varname:
            h = prettyHF(h,isCharm,"MPIref" in name,noplotmpi)
            h.GetYaxis().SetTitle("Cross section (pb)")
          h.GetYaxis().SetTitleSize(0.06)
          h.GetYaxis().SetTitleOffset(0.75)
          h.GetYaxis().SetLabelSize(0.050)
          #h.GetYaxis().SetLabelOffset(0.045)
          if store:
            outfile.cd()
            h.Write()
          h.SetDirectory(0)
          files[name][3] = h

          #------------------- plot and store
          h.SetLineWidth(3)
          h.SetMarkerStyle(4)
          #if "PowhegHerwig" in name:
          #  h.SetLineColor(2)
          #else:
          #  h.SetLineColor(c+1)
          if "PowhegHerwig" in name:
            h.SetLineColor(3)
            h.SetMarkerColor(3)
            h.SetLineStyle(1)
          elif color in colorMap.keys() and c in colorMap[color].keys():
            h.SetLineColor(colorMap[color][c][0])
            h.SetMarkerColor(colorMap[color][c][0])
            h.SetLineStyle(colorMap[color][c][1])
          else:
            h.SetLineColor(c+21)
            h.SetMarkerColor(c+21)
            h.SetLineStyle(1)
          pad_1.cd()
          print h.Integral(0,-1)
          if norm:
            if "extHFtype" in varname:
              h.Scale(1./h.Integral(0,7))
            else:
              h.Scale(1./h.Integral(0,-1))
            h.GetYaxis().SetTitle("Arbitrary units")
          h.SetMaximum(h.GetMaximum()*1.8)
          if not "dr" in varname and not "eta" in varname: 
            h.SetMaximum(h.GetMaximum()*3)
          if "pt" in varname or "_ht" in varname or "_m" in varname: 
            h.SetMaximum(h.GetMaximum()*20)
          if "extHFtype" in varname:
            h.SetMaximum(h.GetMaximum()*10)
            if isCharm and not norm:
              h.SetMinimum(7e-3)
            else:
              h.SetMinimum(2e-4)
          same = ""
          if c!=0: same = "same"
          h.DrawCopy(same)
          nicename = name.replace("Off_"," ").replace("MiniTruth","").replace("Fix","").replace("_HF","").replace("up"," up").replace("down"," down").replace("MPI","")
          nicename = nicename.replace(" nominalwithMEb","+Pythia")
          nicename = nicename.replace("withMEb","")
          nicename = nicename.replace("_ME0b",", 0 b's from ME").replace("_ME1b",", 1 b's from ME").replace("_ME2b",", 2 b's from ME")
          nicename = nicename.replace("Sherpa2_","Sherpa+OpenLoops ").replace("NLO_decayed","default").replace("nonprompt","").replace("Powheg","Powheg+")
          nicename = nicename.replace("defaultX2","#mu_{R} x2").replace("defaultX05","#mu_{R} x0.5")
          nicename = nicename.replace(" default","").replace("Pythiaref","Pythia").replace("Pythiahigh","Pythia High MPI")
          nicename = nicename.replace("p15", "").replace("onlyHF", "").replace("Madgraph_nominal","Madgraph+Pythia")
          nicename = nicename.replace("R_Mbb","#mu_{R}=#sqrt{m_{t}m_{b#bar{b}}}").replace("Q_CMMPS","#mu_{F}=#mu_{Q}=#mu_{R}")
          nicename = nicename.replace("defaultp15b","default").replace("p25no","").replace("Q2","Q^{2}").replace(" Mc "," M_{c} ").replace("Match up","Matching up")
          nicename = nicename.replace("_norw"," no t#bar{t} rw.")
          if  all(["Sherpa" in x for x in names]):
            nicename = nicename.replace("OpenLoops","OL")
          #if(nicename=="Powheg+Pythia"):
          #  #nicename="t#bar{t}+jets Powheg+Pythia"
          #  nicename="POWHEG+PYTHIA"
          #if(nicename=="Powheg+Herwig_norw"):
          #  nicename="t#bar{t}+jets Powheg+fHerwig"
          #if(nicename=="Madgraph+Pythia"):
          #  #nicename="t#bar{t}+jets Madgraph+Pythia"
          #  nicename="MADGRAPH+PYTHIA"
          leg.AddEntry(h,nicename)
          pad_2.cd()
          h2 = h.Clone() 
          h2.Divide(files[reference][3])
          if isMEsplit:
            h2.GetYaxis().SetRangeUser(0.00001,0.99999)
            h2.GetYaxis().SetNdivisions(404)
          else:
            h2.GetYaxis().SetRangeUser(0.00001+0.4*zoomratio,1.99999-0.4*zoomratio)
            if zoomratio:
                h2.GetYaxis().SetNdivisions(404)
            else:
                h2.GetYaxis().SetNdivisions(504)
          if isMEsplit:
            h2.GetYaxis().SetTitle("Fraction over total")
          else:
            h2.GetYaxis().SetTitle("MC / Powheg+Pythia")
          #h2.GetYaxis().SetTitle("MC / SHERPA OL default")
          #h2.SetMaximum(0.99) #FIXME
          #h2.SetMinimum(0.01)
          if "extHFtype" in varname:
            h2.GetXaxis().SetLabelSize(0.13)
          else:
            h2.GetXaxis().SetLabelSize(0.1)
          h2.GetYaxis().SetLabelSize(0.08)
          h2.GetXaxis().SetTitleSize(0.1)
          h2.GetXaxis().SetLabelOffset(0.015)
          h2.GetXaxis().SetTitleOffset(1.2)
          h2.GetYaxis().SetTitleSize(0.085)
          h2.GetYaxis().SetTitleOffset(0.60)
          if c==0 or c==1:
            h2.DrawCopy()
            #for bin in xrange(1,h2.GetNbinsX()+1):
            #  h2.SetBinError(bin,0)
          else:
            h2.DrawCopy(same)
          if band:
            print c,name, band
            if name == band[0]:
              bandh = h2.Clone("bandh")
              bandh.SetDirectory(0)
              bandh.SetFillColor(root.TColor.kBlue-7)
              bandh.SetFillStyle(3344)
              bandh.SetMarkerStyle(1)
              for bin in range(1,bandh.GetNbinsX()+1):
                bandh.SetBinError(bin,0)
            elif name in band:
              for bin in range(1,bandh.GetNbinsX()+1):
                bandh.SetBinError(bin,max(bandh.GetBinError(bin),abs(bandh.GetBinContent(bin)-h2.GetBinContent(bin))))
            

          line = root.TLine(h2.GetBinLowEdge(1),1,h2.GetBinLowEdge(h2.GetNbinsX()+1),1)
          line.SetLineWidth(2)
          line.Draw()
          if not "dr" in varname and not "eta" in varname: 
            pad_1.SetLogy(1);
          else:
            pad_1.SetLogy(0);
          rfile.Close()


          
          #------------------- plot and store
        #end sample
        pad_1.cd()
        tex.DrawLatexNDC(0.14,0.85,"#font[72]{ATLAS} Simulation")
        tex.SetTextSize(0.05)
        tex.DrawLatexNDC(0.14,0.76,"Internal")
        #tex.DrawLatexNDC(0.14,0.76,"J. Montejo, PhD Thesis")
        #leg.AddEntry(root.NULL,"(*) MPI-FSR not split","")
        if isMEsplit:
          leg.SetTextSize(0.038)
        else:
          leg.SetTextSize(0.045)
        leg.Draw()
        plotname = "_".join([prefix,categname,varname])
        if norm: plotname += "_norm"
        #canv.SaveAs("plots_tesis/"+plotname+".eps")
        #canv.SaveAs("plots_tesis/"+plotname+".pdf")
        canv.SaveAs("plots_tesis/"+plotname+".png")
        if band:
          pad_2.cd()
          bandh.Draw("same,E2")
          canv.SaveAs("plots_tesis/"+plotname+"_band.pdf")
          canv.SaveAs("plots_tesis/"+plotname+"_band.eps")
      #end var
    #end categ
  #end chan
  if store:
    outfile.Close()
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

def doSkip(channame, categname, varname):
  
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
  return False

def doCategStr(files, key, reference,cut, isCharm):
  ctgstr = "*("
  if isCharm:
    var,bins = ("extHFtype",         (50,0.5,50.5)   )
  else:
    var,bins = ("((extHFtype-(extHFtype%100))/100.)",         (50,0.5,50.5)   )
  c01  =root.TCanvas("c01")
  c01.cd()
  reffile  = root.TFile.Open(files[reference][0])
  reftree  = reffile.Get("particle_jets")
  #print reftree, reffile
  h1 = root.TH1D("h1","h1",*bins)
  reftree.Draw(var+" >> h1",cut+files[reference][1])
  #print reference, (var+" >> h1",cut+files[reference][1])
  #print h1.Integral()
  h1.SetDirectory(0)
  reffile.Close()
  thefile  = root.TFile.Open(files[key][0])
  thetree   = thefile.Get("particle_jets")
  h2 = root.TH1D("h2","h2",*bins)
  thetree.Draw(var+" >> h2",cut+files[key][1])
  h2.SetDirectory(0)
  thefile.Close()

  print h1.Integral(),h2.Integral()
  #h1.Scale(1./h1.Integral())
  #h2.Scale(1./h2.Integral())
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

def getSyst(name):
  ttbar_nominal = (0.0409119,  -0.0121258,  -0.188448,  -0.316183)
  top_nominal   = (0.0139729, 0.0128711, 0.00951743, 0.00422323, -0.0352631, -0.0873852, -0.120025)
  top_UP_ISRFSR = (-0.0172321, 0.000228524, 0.0190734, 0.0321922, -0.00524551, -0.0606578, -0.0850999 )
  top_DOWN_ISRFSR = (0.0434901, 0.029907, -0.00139654, -0.0250363, -0.0660721, -0.113041, -0.151856 )
  top_UP_MCGenerator = (-0.0015291, 0.0231146, 0.0181124, 0.00602686, -0.0513448, -0.130822, -0.21204 )
  top_DOWN_MCGenerator = (0.0249864, 0.00985491, -0.00107294, -0.00232321, -0.0242983, -0.0455976, -0.0281206 )
  ttbar_UP_ISRFSR = (0.143962,  -0.130473,  -0.332904,  -0.447613)
  ttbar_DOWN_ISRFSR = (-0.0621383,  0.106222,  -0.0439914,  -0.184754)
  ttbar_UP_Fragmentation = (0.0501761,  -0.0150894,  -0.245743,  -0.335193)
  ttbar_DOWN_Fragmentation = (0.0316478,  -0.00916213,  -0.131152,  -0.297173)
  ttbar_UP_MCGenerator = (-0.00134908,  0.025611,  -0.130584,  -0.329586)
  ttbar_DOWN_MCGenerator = (0.083173,  -0.0498626,  -0.246311,  -0.30278)
  if "MCgen" in name:
    if "up" in name:
      top = top_UP_MCGenerator
      ttbar = ttbar_UP_MCGenerator
    elif "down" in name:
      top = top_DOWN_MCGenerator
      ttbar = ttbar_DOWN_MCGenerator
  if "IFSR" in name:
    if "up" in name:
      top = top_UP_ISRFSR
      ttbar = ttbar_UP_ISRFSR
    elif "down" in name:
      top = top_DOWN_ISRFSR
      ttbar = ttbar_DOWN_ISRFSR

  rw = "*((ttbar_pt/1000. >= 0.0 && ttbar_pt/1000. < 40.0)*%s+(ttbar_pt/1000. >= 40.0 && ttbar_pt/1000. < 170.0)*%s+(ttbar_pt/1000. >= 170.0 && ttbar_pt/1000. < 340.0)*%s+(ttbar_pt/1000. >= 340.0 && ttbar_pt/1000. < 1000.0)*%s)*((top_pt/1000. >= 0.0 && top_pt/1000. < 50.0)*%s+(top_pt/1000. >= 50.0 && top_pt/1000. < 100.0)*%s+(top_pt/1000. >= 100.0 && top_pt/1000. < 150.0)*%s+(top_pt/1000. >= 150.0 && top_pt/1000. < 200.0)*%s+(top_pt/1000. >= 200.0 && top_pt/1000. < 250.0)*%s+(top_pt/1000. >= 250.0 && top_pt/1000. < 350.0)*%s+(top_pt/1000. >= 350.0 && top_pt/1000. < 800.0)*%s)" % tuple([str((1+x)/(1+y)) for x,y in zip(ttbar+top,ttbar_nominal+top_nominal)])
  print name, rw
  return rw

bins = []
def prettyHF(original, isCharm, isMPI,noplotmpi):

  global bins
  if not bins:
    intoriginal  = original.Integral()
    for bin in xrange(1,original.GetNbinsX()+1):
      cont1 = original.GetBinContent(bin)
      if not cont1: continue
      if isMPI or noplotmpi:
        if cont1/intoriginal < 1e-4: continue
      elif isCharm:
        if cont1/intoriginal < 7e-4: continue
      else:
        if cont1/intoriginal < 2e-4: continue
      hf   = int(original.GetBinCenter(bin))
      print cont1/intoriginal, hf
      if hf >= 40: continue
      if hf < 0 and noplotmpi: continue
      #if hf == 1: cont1 *= 1.4 #FIXME hack to avoid ttB and ttbb swapping
      label = "t#bar{t} + "
      if hf <= -10: 
        label = "t#bar{t} + MPI "
      elif hf < 0 : label = "t#bar{t} + FSR "
      abs_hf = abs(hf)
      if hf == 0: label = "tt+jets"
      if isCharm: label = label+"c"*(abs_hf/10)+"C"*(abs_hf%10)
      else:       label = label+"b"*(abs_hf/10)+"B"*(abs_hf%10)
      #if hf > 0:  label = "tt"+"b"*(hf/10)+"B"*(hf%10)
      #if hf < 0:  label = "tt"+"c"*(hf/10)+"C"*(hf%10)
      if "bb" in label:
        newlabel=label.replace("bb","b#bar{b}")
        print newlabel
      else:
        newlabel=label
      bins.append((bin,newlabel,hf,cont1))
    tmp  = sorted([(x,y,w,z) for (x,y,w,z) in bins if w <  0] , key=operator.itemgetter(3),reverse=True)
    print tmp
    bins = sorted([(x,y,w,z) for (x,y,w,z) in bins if w >= 0] , key=operator.itemgetter(3),reverse=True)
    print bins
    bins += tmp
    
  print bins
  new = root.TH1D(original.GetName(),original.GetTitle(),len(bins),0,len(bins))
  for i, (bin, label, hf, cont) in enumerate( bins):
    new.SetBinContent(i+1,original.GetBinContent(bin))
    new.SetBinError(i+1,original.GetBinError(bin))
    new.GetXaxis().SetBinLabel(i+1, label)
  new.GetXaxis().LabelsOption("u")
  return new

if __name__ == "__main__":

  parser = OptionParser() 
  parser.add_option("-j", "--jo", 
                    dest="jo", 
                    help="Job option with channels and variables", 
                    default=None)
  parser.add_option("-c", "--color-scheme", 
                    dest="color", 
                    help="Color scheme to be used", 
                    default=0)
  parser.add_option("-z", "--zoom-ratio", 
                    dest="zoomratio", 
                    help="Zoom the ratio panel", 
                    default=False,
                    action="store_true")
  parser.add_option("-p", "--prefix", 
                    dest="prefix", 
                    help="Name to be prepend to the plots", 
                    default="plot")
  parser.add_option("-r", "--reference", 
                    dest="reference", 
                    help="Reference sample", 
                    default="")
  parser.add_option("-s", "--suffix", 
                    dest="suffix", 
                    help="Suffix to append to the samples", 
                    default="")
  parser.add_option("-n", "--norm", 
                    dest="norm", 
                    help="Normalize plots to unity",
                    action="store_true")
  parser.add_option("-m", "--no-mpi", 
                    dest="noplotmpi", 
                    help="Don't plot MPI and FSR bins",
                    action="store_true")
  parser.add_option("-S", "--store", 
                    dest="store", 
                    help="Store histograms in root file",
                    action="store_true")
  parser.add_option("-b", "--band", 
                    dest="band", 
                    action="append",
                    help="Draw a band with center first sample, and width max of next samples", 
                    default=[])

  options,args = parser.parse_args() 
  args += options.band
  if options.reference:
    args.insert(0,options.reference)
  if len(args) < 2:
    print "Need to specify at leaste two files to compare"
    sys.exit(1)
  if options.reference and not options.reference in args:
    print "Reference not known",options.reference
    sys.exit(1)
  if options.suffix:
    args = [arg+options.suffix for arg in args]

  root.gROOT.SetBatch(1)
  root.TH1.SetDefaultSumw2(1)
  root.gStyle.SetOptTitle(0)
  root.gStyle.SetOptStat(0)
  #root.gROOT.ProcessLineSync(".x convert.C+")
  jo = importlib.import_module(options.jo.replace(".py",""))

  main(args,options.prefix,options.reference,options.band,options.norm,options.store,int(options.color),options.noplotmpi,options.zoomratio)
