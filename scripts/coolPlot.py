import ROOT as root
import os.path
import math

root.gStyle.SetLabelFont(43,"XYZ")
root.gStyle.SetLabelSize(20,"XYZ")
root.gStyle.SetTitleFont(43,"XYZ")
root.gStyle.SetTitleSize(20,"XYZ")

def mycolor(h):
	return h+1+1*(h>=4)+30*(h>=8)

class coolPlot(object):

	def __init__(self, name, histolist, titlelist=None, ref=0,canvas=None,normalized=False,folder="plots/mine/",binning=None,yields=False, rebin=0,log=False,yrange=None,yrangeratio=None,plotratio=True,formats=("png",), legendTitle=None, sideline=0, additionals=[], delete=False, sbratio=False):

		todel = []
		if not canvas:
			canvas =	root.TCanvas(name,name, 800, 800 )
			todel.append(canvas)
		if normalized:
			copylist = []
			yieldlist = []
			for hist in histolist:
				copy = hist.Clone()
				yieldlist.append(copy.Integral(0,-1))
				copy.Scale(1./copy.Integral(0,-1))
				copylist.append(copy)
		else:
			copylist = histolist[:]
		if titlelist:
			for i,(title,dummyh) in enumerate(zip(titlelist,copylist)):
				copylist[i].SetTitle(title)
		todel.extend(copylist)
		if rebin>1:
			for hist in copylist:
				hist.Rebin(rebin)
		plotratio = len(copylist)>1 and (plotratio or sbratio)
		if ref:
			href = copylist.pop(ref)
			copylist.insert(0,href)

		if plotratio:
			toppad		= root.TPad("toppad",		"toppad",		0.0, 0.28, 1., 1., 0, 0, 0 )
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
			toppad		= root.TPad("toppad",		"toppad",		0.0, 0.0, 1., 1., 0, 0, 0 )
			todel.append(toppad)
			canvas.cd()
			toppad.Draw()
			toppad.cd()
			toppad.SetTopMargin(0.05)
			toppad.SetBottomMargin(0.14)
			toppad.SetLeftMargin(0.14)
			toppad.SetRightMargin(0.05)
			toppad.SetFrameBorderMode(0)
			canvas.cd()

		legend = root.TLegend( 0.6,0.5,0.9,0.9)
		todel.append(legend)
		legend.SetFillStyle(0)
		legend.SetLineColor(0)
		legend.SetBorderSize(0)
		legend.SetShadowColor(10)
		if yields :
				legend.SetNColumns(2)
				legend.SetTextAlign(32)
				legend.SetColumnSeparation(-0.1)
				pass

		toppad.cd()
		for h,histo in enumerate(copylist[:]):
			same = "same"
			if h==0: same=""
			if len(copylist)>1:
				histo.SetLineColor(mycolor(h))
			histo.Draw("hist,"+same)
			#histo.Draw("FUNC,same")
			legend.AddEntry(histo)
			if yields:
				if normalized:
					legend.AddEntry(root.NULL, "%d" % (yieldlist[h]), "")
				else:
					legend.AddEntry(root.NULL, "%d" % (histo.Integral(0,-1)), "")
			if h==0 and yrange:
				histo.GetYaxis().SetRangeUser(yrange[0],yrange[1])
		if len(copylist)>1 or legendTitle:
			if legendTitle: legend.SetHeader(legendTitle)
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
			
			
		if plotratio or sbratio:
			bottompad.cd()
			if sbratio:
				total = None
				for histo in copylist[:1]: #FIXME
					if not total: total = histo.Clone("Total")
					else: total.Add(histo)
			cumratio = 0
			for h,histo in enumerate(copylist):
				if h==0: continue
				same = "same"
				if h==1: same=""
				ratio = histo.Clone()
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
				else:
					ratio.Divide(copylist[0])
				ratio = ratio.DrawCopy(same)
				if h==1:
					if yrangeratio:
						ratio.GetYaxis().SetRangeUser(yrangeratio[0],yrangeratio[1])
					else:
						ratio.GetYaxis().SetRangeUser(0.45,1.55)
				ratio.GetYaxis().SetTitle("")
				ratio.GetXaxis().SetTitleOffset(4)
			print "Average improvement:",cumratio/(len(copylist)-1)
			line = root.TLine(ratio.GetBinLowEdge(1),1,ratio.GetBinLowEdge(ratio.GetNbinsX()+1),1)
			line.SetLineColor(root.kGray)
			line.Draw("same")
			todel.append(line)
				
		for f in formats:
			canvas.SaveAs(os.path.join(folder,name+"."+f))

		if delete:
			for item in todel: del item
