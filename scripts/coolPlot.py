import ROOT as root
import os.path
import math

mygit = os. environ['MYGIT']
root.gROOT.LoadMacro(mygit+"/atlasrootstyle/AtlasStyle.C")
root.gROOT.LoadMacro(mygit+"/atlasrootstyle/AtlasUtils.C")
root.SetAtlasStyle()
root.gStyle.SetOptTitle(0)
root.gStyle.SetOptStat(0)
root.gStyle.SetOptFit(0)
root.gStyle.SetHistLineWidth(2)
root.gStyle.SetLabelFont(43,"XYZ")
root.gStyle.SetLabelSize(30,"XYZ")
root.gStyle.SetTitleFont(43,"XYZ")
root.gStyle.SetTitleSize(30,"XYZ")
root.gROOT.SetBatch(1)

Colors = [
    root.kBlack,
    root.kRed,
    root.kBlue,
    root.kGreen + 1,
    root.kOrange + 1,
    #root.kGray + 3,
    root.kRed - 2,
    root.kBlue - 9,
    root.kGreen - 6,
    root.kOrange - 3,
    root.kGray,
    root.kMagenta + 2,
    root.kCyan,
    root.kSpring + 10,
    root.kOrange + 10,
    root.kViolet - 3,
    root.kOrange - 9,
    root.kTeal + 3,
    root.kPink + 10,
]
def mycolor(h,dummy):
    if dummy: h -= 1
    return Colors[h]
def mymarker(h):
    return h+20

class coolPlot(object):

    def __init__(self, name, histolist, titlelist=None, ref=0,canvas=None,normalized=False,folder="plots/mine/",binning=None,yields=False, rebin=0,log=False,rangex=None,yrange=None,yrangeratio=[0.45,1.55],plotratio=True,formats=("png",), legendTitle=None, sideline=0, additionals=[], delete=False, sbratio=False,ratiofunction="", legendcoord=( 0.6,0.6,0.9,0.9), separation=False, overflow=True, statuncertainty=False, forcehistoption=False, doMarkers=False, xtitle=None, dummyRef=None, noLegend=False):
        '''
            dummyRef is used when plotting non-histogram objects such as TEfficiency. Use the dummyRef to get a histogram-like object to draw and then not display it
        '''


        self.separation = []
        todel = []
        if not canvas:
            canvas =    root.TCanvas(name,name, 800, 800 )
            todel.append(canvas)
        if normalized:
            copylist = []
            yieldlist = []
            for hist in histolist:
                copy = hist.Clone()
                todel.append(copy)
                yieldlist.append(copy.Integral(0,-1))
                if copy.Integral(0,-1):
                    copy.Scale(1./copy.Integral(0,-1))
                copylist.append(copy)
        else:
            copylist = [h.Clone() for h in histolist]
        if dummyRef:
            #dummyRef = dummyRef.Clone()
            dummyRef.Reset()
            copylist = [dummyRef]+copylist
        if overflow:
          for hist in copylist:
            lastbin = hist.GetNbinsX()
            hist.SetBinContent(lastbin, hist.GetBinContent(lastbin)+hist.GetBinContent(lastbin+1))
            hist.SetBinError(lastbin, math.sqrt(pow(hist.GetBinError(lastbin),2)+pow(hist.GetBinError(lastbin+1),2)))
            hist.SetBinContent(1, hist.GetBinContent(1)+hist.GetBinContent(0))
            hist.SetBinError(1, math.sqrt(pow(hist.GetBinError(1),2)+pow(hist.GetBinError(0),2)))
        if titlelist:
            if dummyRef:
                titlelist = ["dummyRef"]+titlelist
            for i,title in enumerate(titlelist):
                copylist[i].SetTitle(title)
        todel.extend(copylist)
        if rebin>1:
            for hist in copylist:
                hist.Rebin(rebin)
        plotratio = (len(copylist)>1 and (plotratio)) or sbratio  or ratiofunction
        if ref:
            href = copylist.pop(ref)
            copylist.insert(0,href)

        if plotratio:
            toppad    = root.TPad("toppad",    "toppad",    0.0, 0.28, 1., 1., 0, 0, 0 )
            bottompad = root.TPad("bottompad", "bottompad", 0.0, 0., 1., 0.28, 0, 0, 0) 
            todel.append(toppad)
            todel.append(bottompad)
            canvas.cd()
            toppad.Draw()
            toppad.cd()
            toppad.SetTopMargin(0.05)
            toppad.SetBottomMargin(0.00)
            toppad.SetLeftMargin(0.14)
            toppad.SetRightMargin(0.05)
            toppad.SetFrameBorderMode(0)
            canvas.cd()
            bottompad.Draw()
            bottompad.cd()
            bottompad.SetTopMargin(0.00)
            bottompad.SetBottomMargin(0.37)
            bottompad.SetLeftMargin(0.14)
            bottompad.SetRightMargin(0.05)
            bottompad.SetFrameBorderMode(0)
        else:
            toppad      = root.TPad("toppad",       "toppad",       0.0, 0.0, 1., 1., 0, 0, 0 )
            todel.append(toppad)
            canvas.cd()
            toppad.Draw()
            toppad.cd()
            toppad.SetTopMargin(0.03)
            toppad.SetBottomMargin(0.13)
            toppad.SetLeftMargin(0.13)
            toppad.SetRightMargin(0.04)
            toppad.SetFrameBorderMode(0)
        canvas.cd() #this fucker avoids a crash in garbage collector O_O

        legend = root.TLegend(*legendcoord)
        todel.append(legend)
        legend.SetTextFont(42)
        legend.SetFillStyle(0)
        legend.SetLineColor(0)
        legend.SetBorderSize(0)
        legend.SetShadowColor(0)
        if yields :
                legend.SetNColumns(2)
                legend.SetTextAlign(32)
                legend.SetColumnSeparation(-0.1)
                pass

        toppad.cd()
        hmax = 0
        for h,histo in enumerate(copylist):
            same = "same"
            if h==0 and dummyRef: same="AXIS"
            if h==0 and not dummyRef: same=""
            if statuncertainty: same+=",e"
            if forcehistoption: same+=",hist"
            if len(copylist)>1:
                histo.SetLineColor(mycolor(h,dummyRef))
                histo.SetMarkerColor(mycolor(h,dummyRef))
                if doMarkers:
                    histo.SetMarkerStyle(mymarker(h))
            histo.Draw(same)
            if xtitle and h==0:
                histo.GetXaxis().SetTitle(xtitle)
            if not yrange: hmax = max(hmax, histo.GetMaximum())
            if ratiofunction:
                histo.GetFunction(ratiofunction).SetLineColor(mycolor(h,dummyRef))
                histo.Draw("FUNC,same")
            if not (h==0 and dummyRef):
                legend.AddEntry(histo)
            if yields:
                if normalized:
                    legend.AddEntry(root.NULL, "%d" % (yieldlist[h]), "")
                else:
                    legend.AddEntry(root.NULL, "%d" % (histo.Integral(0,-1)), "")
            if h==0 and rangex:
                histo.GetXaxis().SetRangeUser(rangex[0],rangex[1])
            if h==0 and yrange:
                histo.GetYaxis().SetRangeUser(yrange[0],yrange[1])
            if separation:
                self.separation.append(getSeparation(histo, copylist[0]))
            #hcum = histo.GetCumulative()
            #print(name, h, histo.GetName())
            #for b in range(1,hcum.GetNbinsX()+1):
            #    print(b,hcum.GetBinLowEdge(b),hcum.GetBinContent(b),1-hcum.GetBinContent(b))
        if not yrange:
            copylist[0].SetMaximum(hmax *(1.2 if not log else 10))
            copylist[0].SetMinimum(0)
                
        if len(copylist)>1 or legendTitle:
            if legendTitle: legend.SetHeader(legendTitle)
            if not noLegend:
                legend.Draw("same")
        
        if log:
            toppad.SetLogy(1)
        if sideline:
            lineup = root.TLine(histo.GetBinLowEdge(1),1+sideline,histo.GetBinLowEdge(histo.GetNbinsX()+1),1+sideline)
            lineup.SetLineColor(root.kGray)
            lineup.Draw("same")
            linedo = root.TLine(histo.GetBinLowEdge(1),1-sideline,histo.GetBinLowEdge(histo.GetNbinsX()+1),1-sideline)
            linedo.SetLineColor(root.kGray)
            linedo.Draw("same")
            todel.append(lineup)
            todel.append(linedo)
        if additionals: 
            toppad.cd()
            for x in additionals:
                x.Draw("same")
            
            
        if plotratio:
            bottompad.cd()
            if sbratio:
                total = None
                for histo in copylist[:1]: #FIXME
                    if not total: 
                        total = histo.Clone("Total")
                        todel.append(total)
                    else: total.Add(histo)
            cumratio = 0
            for h,histo in enumerate(copylist):
                if h==0 and not ratiofunction: continue
                same = "same"
                isfirst = (h==0 and     ratiofunction) or (h==1 and not ratiofunction)
                if isfirst: same=""
                if forcehistoption: same+=",hist"
                ratio = histo.Clone()
                if titlelist:
                    ratio.SetTitle(titlelist[h])
                todel.append(ratio)
                if sbratio:
                    best = 0
                    for b in range(ratio.GetNbinsX()+1):
                        if total.Integral(b,-1)<=0: break
                        ssqrtb = ratio.Integral(b,-1)/math.sqrt(total.Integral(b,-1))
                        if b==1: initial = ssqrtb
                        if ssqrtb > best: 
                            best = ssqrtb
                        ratio.SetBinContent(b, ssqrtb)
                    cumratio += best/initial
                elif ratiofunction:
                    f = ratio.GetFunction(ratiofunction)
                    f.SetBit(root.TF1.kNotDraw)
                    ratio.Divide(f)
                else:
                    ratio.Divide(copylist[0])
                ratio.Draw(same)
                if isfirst:
                    ratio.GetYaxis().SetRangeUser(yrangeratio[0],yrangeratio[1])
                    if rangex:
                        ratio.GetXaxis().SetRangeUser(rangex[0],rangex[1])
                ratio.GetYaxis().SetTitle("")
                ratio.GetXaxis().SetTitleOffset(4)

            if rangex:
                xmin,xmax = rangex[0],rangex[1]
            else:
                xmin,xmax = ratio.GetBinLowEdge(1), ratio.GetBinLowEdge(ratio.GetNbinsX()+1)
            line = root.TLine(xmin,1,xmax,1)
            line.SetLineColor(root.kGray)
            line.Draw("same")
            todel.append(line)
                
        if not os.path.exists(folder):
            os.makedirs(folder)
        for f in formats:
            canvas.SaveAs(os.path.join(folder,name+"."+f))

        if delete:
            for item in todel: del item

def getSeparation(B1, S1):
    ## keeping TMVA definition
    ##  <s2> = (1/2) Int_-oo..+oo { (S^2(x) - B^2(x))/(S(x) + B(x)) dx }
    sep = 0
    if not S1.Integral() or not B1.Integral(): return -1

    S = S1.Clone(S1.GetName()+"sepS")
    S.Scale(1. / S.Integral())
    B = B1.Clone(B1.GetName()+"sepB")
    B.Scale(1. / B.Integral())

    ## sanity checks: signal and background histograms must have same number of bins and same limits
    if S.GetNbinsX() != B.GetNbinsX() or S.GetNbinsX() <= 0:
        print("signal and bkg samples with different number of bins: S(" + str(
            S.GetNbinsX()) + ") B(" + str(B.GetNbinsX()) + ")")
        return 0

    if S.GetXaxis().GetXmin() != B.GetXaxis().GetXmin() or S.GetXaxis().GetXmax(
    ) != B.GetXaxis().GetXmax() or S.GetXaxis().GetXmax() <= S.GetXaxis(
    ).GetXmin():
        print("Edges of histos are not right: Smin(" + str(S.GetXaxis().GetXmin()) + ")  Bmin(" + str( B.GetXaxis().GetXmin() ) + " ) "\
            " Smax(" + str( S.GetXaxis().GetXmax()) + ")  Bmax(" + str(B.GetXaxis().GetXmax()))
        return 0

    nstep = S.GetNbinsX()
    intBin = (S.GetXaxis().GetXmax() - S.GetXaxis().GetXmin()) / nstep
    nS = S.GetSumOfWeights() * intBin
    nB = B.GetSumOfWeights() * intBin

    if nS > 0 and nB > 0:
        for bin in range(0, nstep):
            s = S.GetBinContent(bin) / nS
            b = B.GetBinContent(bin) / nB
            if s + b > 0: sep += 0.5 * (s - b) * (s - b) / (s + b)
            pass
        sep *= intBin
    else:
        print("histos with 0 entries: Snb(" + str(nS) + ") Bnb(" + str(nB) + ")")
        sep = 0
    del S
    del B
    return sep
