import ROOT as root
import array

root.gROOT.SetBatch(1)
root.TH1.SetDefaultSumw2(1)
root.gStyle.SetOptTitle(0)
root.gStyle.SetOptStat(0)

outfile = root.TFile.Open("analyze_truth_outfiles/outhistos_HFsplit.root","update")

sel_map = {0:"Light",
           -1:"tt+c",-2:"tt+C",-11:"tt+cc",-12:"tt+cC",-22:"tt+CC",
            1:"tt+b", 2:"tt+B", 11:"tt+bb", 12:"tt+bB", 22:"tt+BB"
          }
sel_all = (0,-12,-11,-2,-1,12,11,2,1)
sel_c = (-1,-2,-11,-12,-22)
sel_b = ( 1, 2, 11, 12, 22)

selections = {
              #"all":sel_all,
              "C":sel_c,"B":sel_b}

vars = {
        #"jet_n" :"jet_n",
        #"lep_pt":"lep_pt/1000.",
        #"lep_eta":"abs(lep_eta)",
        #"leadjet_pt":"jet_pt[0]/1000.",
        #"leadjet_eta":"abs(jet_eta[0])",
        "ht_add":"Sum$(jet_pt*(jet_trueflav!=0 && jet_id==0))/1000.",
        "ht":"Sum$(jet_pt)/1000.",
        "lead_add":"Max$(jet_pt*(jet_trueflav!=0 && jet_id==0))/1000.",
}
toppt_bins = array.array("d",[0,50,100,150,200,250,350,800])
ttbarpt_bins = array.array("d",[0,40,170,340,1000])

samples = {
            #"AlpgenHFOR":("Alpgen","(mc_channel_number!=0 && hfor!=4)"),
            #"AlpgenHFtype":("Alpgen","(mc_channel_number!=0 && passHFtype)"),
            #"AlpgenTuned":("Alpgen","(mc_channel_number!=0 && passHFtype)*ktfac2*rw_ttbarpt_toppt"),
            #"MadgraphKtfac2":("MadgraphKtfac2","(1)"),
            #"Madgraph5F":("Madgraph5F","(1)"),
            #"Madgraph_CT10":("Madgraph_CT10","(1)"),
            #"Madgraph_CTEQ6":("Madgraph_CTEQ6","(1)"),
            #"Sherpa":("Sherpa","(1)"),
            "PowhegPythia":("PowhegPythia","(1)"),
            #"PowhegHerwig":("PowhegJimmy_CT10_nom","(1)"),
            #"PowhegPythia_7TeV":("PowhegPythia_7TeV","(1)"),
            }
#for i in ("Jimmy","Pythia"):
#  for j in ("nom","rensc05","rensc2","facsc05","facsc2"):
#    name = "Powheg"+i+"_CT10_"+j
#    samples[name]=(name,"(1)")

c0 = root.TCanvas("c0")
c1 = root.TCanvas("c1")

for varname, var in vars.iteritems():
  print varname,var
  for i,(sample,(file,samplecut)) in enumerate(samples.iteritems()):
    print "- ",i,sample,file,samplecut
    rfile = root.TFile.Open("analyze_truth_outfiles/"+file+".root")
    tree  = rfile.Get("particle_jets")
    for jet, jetname in zip(("","(jet_n==6)*"),("incl","6j")):
      print "- - ",jet, jetname
    #for jet, jetname in zip(("",),("incl",)):
      #for lep, lepname in zip(("2",), ("dilep",)):
      for lep, lepname in zip(("1","2"), ("semilep","dilep")):
        print "- - - ",lep, lepname
        hmap = dict()
        for selname,selection in selections.iteritems():
          print "- - - - ",selname, selection
         
          leg = root.TLegend(0.55,0.55,0.89,0.89)
          leg.SetFillColor(0)
          leg.SetLineColor(0)
          for s,sel in enumerate(selection):
            print "- - - - - ",s, sel
            c0.cd()
            cut = jet+"(lep_n=="+lep+")*(HFtype=="+str(sel)+")*"+samplecut
            weight = "*xsec"

            hname = varname+"_"+sample+"_"+sel_map[sel]+"_"+lepname+"_"+jetname
            if hname in hmap.keys():
              h = hmap[hname]
            else:
              if "dr"      in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",60,0,6)
              elif "HFtype"  in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",45,-22.5,22.5)
              elif "bjet_n"  in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",10,-0.5,9.5)
              elif "jet_n"   in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",20,-0.5,19.5)
              elif "ttbar_pt_binned" in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",len(ttbarpt_bins)-1,ttbarpt_bins)
              elif "ttbar_pt" in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",50,0,1000)
              elif "top_pt_binned" in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",len(toppt_bins)-1,toppt_bins)
              elif "top_pt" in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",50,0,800)
              elif "sqrt"  in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",50,-10,140)
              elif "pt"    in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",30,0,240)
              elif "ht"    in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",20,0,800)
              elif "eta"   in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",50,0,5)
              elif "phi"   in varname: h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",50,-3.14,3.14)
              else               : h = root.TH1F(hname,hname.replace("_"," ")+";"+varname.replace("_"," ")+";events/pb",50,-10,240)
  
              tree.Draw(var+" >> "+hname,cut+weight)
              if h.Integral(0,-1):
                h.Scale(100./h.Integral(0,-1))
              hmap[hname] = h
            print hname, h.Integral(0,-1)
            if s==4: s=6
            h.SetLineColor(s+1)
            h.SetLineWidth(2)
            if "all" in selname and sel<0: h.SetLineStyle(2)
            c1.cd()
            same = "E"
            if s!=0: same = "E,same"
            h.DrawCopy(same)
            h.SetDirectory(0)
            leg.AddEntry(hmap[hname],sel_map[sel])

            outfile.cd()
            h.Write()
          c1.cd()
          leg.Draw()
          c1.SaveAs("plots/HFsplit_"+lepname+"_"+jetname+"_"+selname+"_"+sample+"_"+varname+".png")
        #end for selections
      #end for leptons
    #end for jets
    rfile.Close()
  #end for samples
#end for vars
outfile.Close()
