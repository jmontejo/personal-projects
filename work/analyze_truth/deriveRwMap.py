from optparse import OptionParser
import ROOT as root
import array, os, sys
import operator

categories = {
        #"inclusive": ("(1.)",2,"inclusive"),
        "realHFbb": ("(extHFtype>=100)",2,"inclusive"),
        "realHFcc": ("(extHFtype<100 && extHFtype>0)",2,"inclusive"),
}

def main(names,reference,systematic,norm):
  
  files = {}
  for name in names:
    file = "analyze_truth_outfiles/"+name.replace("_IFSR","").replace("_MCgen","").replace("_up","").replace("_down","").replace("_lepjets","").replace("_dilepton","").replace("_dimuon","").replace("_singlemuon","").replace("_norw","")+".root"
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
    if "ME0b" in name:
      weight += "*(num_ME_b==0)"
    if "ME1b" in name:
      weight += "*(num_ME_b==1)"
    if "ME2b" in name:
      weight += "*(num_ME_b>=2)"
    if "norw" in name:
      weight = weight.replace("*rw_ttbarpt_toppt","")

    files[name] = [file,weight,"",None]
  if not reference: reference = names[0]
  keys = [reference]
  for name in files.keys():
    if name not in keys:
      keys.append(name)
  keys = names

  for categname, (categ, rebin, categnamepretty) in categories.iteritems():
    print categname
    cut = categ
    isCharm =False
    if "c" in categname: isCharm = True
    if isCharm and "Sherpa" in reference: continue
    for key in files.iterkeys():
      if key == reference: continue
      categNorm = doCategStr(files,key,reference,cut,isCharm,systematic,norm)
  #end categ
#-----------------------

def doCategStr(files, key, reference,cut, isCharm, systematic,norm):
  if isCharm:
    var,bins = ("extHFtype",         (50,0.5,50.5)   )
  else:
    var,bins = ("((extHFtype-(extHFtype%100))/100.)",         (50,0.5,50.5)   )
  c01  =root.TCanvas("c01")
  c01.cd()
  reffile  = root.TFile.Open(files[reference][0])
  reftree  = reffile.Get("particle_jets")
  h1 = root.TH1D("h1","h1",*bins)
  reftree.Draw(var+" >> h1",cut+files[reference][1])
  h1.SetDirectory(0)
  reffile.Close()
  thefile  = root.TFile.Open(files[key][0])
  thetree   = thefile.Get("particle_jets")
  h2 = root.TH1D("h2","h2",*bins)
  thetree.Draw(var+" >> h2",cut+files[key][1])
  h2.SetDirectory(0)
  thefile.Close()

  print "Reference: ",h1.Integral()
  print "Other:     ",h2.Integral()
  if norm:
    h1.Scale(1./h1.Integral())
    h2.Scale(1./h2.Integral())
  h1.Divide(h2)
  if not isCharm:
    for bin in range(1,h1.GetNbinsX()+1):
      if h1.GetBinContent(bin) == 0: continue
      print "%s_HFcateg[%d] = %f;" % (systematic,int(h1.GetBinLowEdge(bin)+0.5),h1.GetBinContent(bin))
  del h1, h2

if __name__ == "__main__":

  parser = OptionParser() 
  parser.add_option("-r", "--reference", 
                    dest="reference", 
                    help="Reference sample", 
                    default="")
  parser.add_option("-s", "--systematic", 
                    dest="systematic", 
                    help="Systematic name",
                    default="")
  #parser.add_option("-n", "--norm", 
  #                  dest="norm", 
  #                  action="store_true", 
  #                  help="Normalize the reweighting map to preserve yields",
  #                  default=False)

  options,args = parser.parse_args() 
  options.norm = True
  if options.reference:
    args.insert(0,options.reference)
  if len(args) < 2:
    print "Need to specify at leaste two files to compare"
    sys.exit(1)
  if options.reference and not options.reference in args:
    print "Reference not known",options.reference
    sys.exit(1)

  root.gROOT.SetBatch(1)
  root.TH1.SetDefaultSumw2(1)
  root.gStyle.SetOptTitle(0)
  root.gStyle.SetOptStat(0)
  #root.gROOT.ProcessLineSync(".x convert.C+")

  main(args,options.reference,options.systematic, options.norm)
