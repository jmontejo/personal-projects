#! /bin/env python

import math as math
import sys as sys
import ROOT as r
import os
from sys import stdout
from array import array
from optparse import OptionParser
#import rootlogon

#r.gROOT.SetStyle("ATLAS")
#r.gROOT.ForceStyle()
r.gStyle.SetOptTitle(0)
#TGaxis.SetMaxDigits(4)
r.gErrorIgnoreLevel = 1001
r.gROOT.SetBatch(1)
r.TH1.SetDefaultSumw2(1)

GEV=1000.

drawData = True
drawSignal = True

basepath=""

isWBXmreco = False

#################################
# General Definitions
#################################
variableLabelsAndUnits = {
    "MET":  ["E_{T}^{miss}", "GeV"],
    "Wlep_MassT":  ["M_{T}^{lep W}", "GeV"],
    "Wlep_pt":  ["p_{T}^{lep W}", "GeV"],
    "Whad_pT":  ["p_{T}^{had W}", "GeV"],
    "deltaPhi_lep_MET": ["#Delta#phi(lep,E_{T}^{miss})","rad"],
    "LepPt":  ["p_{T}^{lepton}", "GeV"],
    "JetPt1":  ["p_{T}^{leading jet}", "GeV"],
    "JetPt2":  ["p_{T}^{2nd leading jet}", "GeV"],
    "JetPt3":  ["p_{T}^{3rd leading jet}", "GeV"],
    "JetPt4":  ["p_{T}^{4th leading jet}", "GeV"],
    "JetPt5":  ["p_{T}^{5th leading jet}", "GeV"],
    "JetPt6":  ["p_{T}^{6th leading jet}", "GeV"],
    "JetPtB1":  ["p_{T}^{leading b jet}", "GeV"],
    "JetPtB2":  ["p_{T}^{2nd leading b jet}", "GeV"],
    "JetPtB3":  ["p_{T}^{3rd leading b jet}", "GeV"],
    "JetPtB4":  ["p_{T}^{4th leading b jet}", "GeV"],
#     "JetPtB1":  ["p_{T}^{highest MV1 weight}", "GeV"],
#     "JetPtB2":  ["p_{T}^{2nd highest MV1 weight}", "GeV"],
    "NN125_4j_2b_nob":  ["NN output", ""],
    "NN125_5j_2b_nob":  ["NN output", ""],
    "NN125_5j_3b_nob":  ["NN output", ""],
    "NN125_6j_2b_nob":  ["NN output", ""],
    "NN125_6j_ge4b_kin":  ["NN output", ""],
    "HTAll":  ["H_{T}", "GeV"],
    "HTnolep":  ["H_{T}^{nolep}", "GeV"],
    "mbb_minDR":  ["M_{bb}^{min #Delta R}", "GeV"],
    "MinDR_LepJ":  ["#Delta R_{lepton jet}", ""],
    "MinDR_LepB":  ["#Delta R_{lepton b-jet}", ""],
    "NoMinDR_bb":  ["#Delta R_{bb}^{no min #Delta R}", ""],
    "NoMinDR_bb_Mass":  ["M_{bb}^{no min #Delta R}", ""],
    "NoMinDR_bb_Pt":  ["pT_{bb}^{no min #Delta R}", ""],
    "MinDR_bb":  ["#Delta R_{bb}^{min #Delta R}", ""],
    "MinDR_bb_Mass":  ["M_{bb}^{min #Delta R}", ""],
    "MinDR_bb_Pt":  ["pT_{bb}^{min #Delta R}", ""],
    "MV1_4_MinDR_bb":  ["#Delta R_{bb}^{min #Delta R} 4 highest MV1", ""],
    "MV1_4_MinDR_bb_Mass":  ["M_{bb}^{min #Delta R} 4 highest MV1", ""],
    "MV1_4_MinDR_bb_Pt":  ["pT_{bb}^{min #Delta R} 4 highest MV1", ""],
    "HT2All":  ["H_{T}^{all}(2)", "GeV"],
    "HT3All":  ["H_{T}^{all}(3)", "GeV"],
    "HT4All":  ["H_{T}^{all}(4)", "GeV"],
    "HT5All":  ["H_{T}^{all}(5)", "GeV"],
    "HT6All":  ["H_{T}^{all}(6)", "GeV"],
    "HT7All":  ["H_{T}^{all}(7)", "GeV"],
    "HTHad":  ["H_{T}^{had}", "GeV"],
    "HTj":  ["H_{T}^{had}", "GeV"],
    "HT2Had":  ["H_{T}^{had}(2)", "GeV"],
    "HT3Had":  ["H_{T}^{had}(3)", "GeV"],
    "HT4Had":  ["H_{T}^{had}(4)", "GeV"],
    "HT5Had":  ["H_{T}^{had}(5)", "GeV"],
    "HT6Had":  ["H_{T}^{had}(6)", "GeV"],
    "HT7Had":  ["H_{T}^{had}(7)", "GeV"],
    "LepEta":  ["#eta^{lepton}", ""],
    "JetEta1":  ["#eta^{leading jet}", ""],
    "JetEta2":  ["#eta^{2nd leading jet}", ""],
    "JetEta3":  ["#eta^{3rd leading jet}", ""],
    "JetEta4":  ["#eta^{4th leading jet}", ""],
    "JetEta5":  ["#eta^{5th leading jet}", ""],
    "JetEta6":  ["#eta^{6th leading jet}", ""],
    "nWhad":  ["Number of W_{had} candidates selected", ""],
    "VLQAna_WbX_MinDRlb":  ["min #DeltaR(l,b)", ""],
    "VLQAna_WbX_MinDRWb":  ["min #DeltaR(W,b)", ""],  
    "VLQAna_WbX_DRb1b2":   ["DR(b_1,b_2)", ""],
    "VLQAna_WbX_DRLepMet": ["#DeltaR(l,#nu)", ""],
    "VLQAna_WbX_W1Pt":     ["p_T(W)", ""],
    "VLQAna_WbX_1W_MWb_1": ["m_{reco1}", ""],
    "VLQAna_WbX_1W_MWb_2": ["m_{reco2}", ""],
    "VLQAna_WbX_1W_MWb_3": ["m_{reco3}", ""],
    "VLQAna_WbX_1W_MWb_4": ["m_{reco}", "GeV"],
    #"VLQAna_WbX_1W_MWb_4": ["m_{reco4}", ""],
    "VLQAna_WbX_1W_MWb_5": ["m_{reco5}", ""],
    "VLQAna_WbX_WpreselType1_Pt": ["W_{had}^{type I} p_T", "GeV"],
    "VLQAna_WbX_WpreselType2_Pt": ["W_{had}^{type II} p_T", "GeV"],
    "VLQAna_WbX_WpreselType1_M": ["W_{had}^{type I} candidate mass", "GeV"],
    "VLQAna_WbX_WpreselType2_M": ["W_{had}^{type II} candidate mass", "GeV"],
    "VLQAna_WbX_WpreselType2_DR": ["W_{had}^{type II} DR", ""],
    "VLQAna_WbX_WpreselType": ["W{had} type", ""],
#     "VLQAna_WbX_WpreselType1_Pt": ["W_{type I} p_T", "GeV"],
#     "VLQAna_WbX_WpreselType2_Pt": ["W_{type II} p_T", "GeV"],
#     "VLQAna_WbX_WpreselType1_M": ["W_{type I} M", "GeV"],
#     "VLQAna_WbX_WpreselType2_M": ["W_{type II} M", "GeV"],
#     "VLQAna_WbX_WpreselType2_DR": ["W_{type II} DR", ""],
    "JetBtagW1mW2":  ["b-tag weight^{highest MV1 weight} - b-tag weight^{2nd highest MV1 weight}", ""],
    "JetBtagW1":  ["b-tag weight^{highest MV1 weight}", ""],
    "JetBtagW2":  ["b-tag weight^{2nd highest MV1 weight}", ""],  
    "JetBtagW2mW3":  ["b-tag weight^{2nd highest MV1 weight} -b-tag weight^{3rd highest MV1 weight}", ""],  
    "JetBtagW3":  ["b-tag weight^{3rd highest MV1 weight}", ""],  
    "JetBtagW4":  ["b-tag weight^{4th highest MV1 weight}", ""],
    "DiscreteBtagW1":  ["b-tag weight^{leading jet}", ""],
    "DiscreteBtagW2":  ["b-tag weight^{2nd leading jet}", ""],  
    "DiscreteBtagW3":  ["b-tag weight^{3rd leading jet}", ""],  
    "DiscreteBtagW4":  ["b-tag weight^{4th leading jet}", ""],
    "Nbjets" :  ["Nbjets", ""],
    "Njets25":  ["Njets25", ""],
    "Njets30":  ["Njets30", ""],
    "Njets35":  ["Njets35", ""],
    "Njets40":  ["Njets40", ""],
    "Njets45":  ["Njets45", ""],
    "Njets50":  ["Njets50", ""],
    "Njets60":  ["Njets60", ""],  
    "SumTagBin":  ["#Sigma btag bin (OP)", ""],
    "ProdTagBin":  ["#Pi btag bin (OP)", ""],
    "Mu"  :["#mu", ""],
    "NPV"  :["# PV", ""],
    "MV1o_JetPt1":  ["p_{T}^{MV1 leading jet}", "GeV"],
    "MV1o_JetPt2":  ["p_{T}^{2nd MV1 leading jet}", "GeV"],
    "MV1o_JetPt3":  ["p_{T}^{3rd MV1 leading jet}", "GeV"],
    "MV1o_JetPt4":  ["p_{T}^{4th MV1 leading jet}", "GeV"],
    "MV1o_JetEta1":  ["#eta^{MV1 leading jet}", ""],
    "MV1o_JetEta2":  ["#eta^{2nd MV1 leading jet}", ""],
    "MV1o_JetEta3":  ["#eta^{3rd MV1 leading jet}", ""],
    "MV1o_JetEta4":  ["#eta^{4th MV1 leading jet}", ""],
    "MV1o_DiscreteBtagW1":  ["b-tag weight^{MV1 leading jet}", ""],
    "MV1o_DiscreteBtagW2":  ["b-tag weight^{2nd MV1 leading jet}", ""],  
    "MV1o_DiscreteBtagW3":  ["b-tag weight^{3rd MV1 leading jet}", ""],  
    "MV1o_DiscreteBtagW4":  ["b-tag weight^{4th MV1 leading jet}", ""],
    "LepD0": ["d_{0}",""],
    "LepD0sig": ["d_{0} significance",""],
    "LepZ0": ["z_{0}",""],
    "LepZ0sig": ["z_{0} significance",""],
    "MuEtcone20": ["E_{T}^{cone 20} (#mu)","GeV"],
    "MuPtcone30": ["p_{T}^{cone 30} (#mu)","GeV"],
    "MuMiniIso": ["mini isolation","GeV"],
    "NN": ["NN output",""],
    "DataPeriod": ["data period",""],
    }
noDensityHist=["DiscreteBtagW1", "DiscreteBtagW2", "DiscreteBtagW3", "DiscreteBtagW4","MV1o_DiscreteBtagW1", "MV1o_DiscreteBtagW2", "MV1o_DiscreteBtagW3", "MV1o_DiscreteBtagW4",]

sampleLegendAndColors = {
    "NonTop"    : ["Non-t#bar{t}", r.TColor.kGray],
    "mergedNonTop": ["Non-t#bar{t}", r.TColor.kGray],
    "ttbar"     : ["t#bar{t}",          0],
    "singleTop" : ["Single top", 62],
    "st"        : ["Single top", 62],
    "Dibosons"  : ["Diboson",    94],
    "db"        : ["Diboson",    94],
    "Wjets_HFOR": ["W+jets",     92],
    "WjetsPythia_HFOR": ["W+jets",     92],
    "WjetsPythiaAuto_HFOR": ["W+jets",     92],
    "Wjets"     : ["W+jets",     92],
    "wj"        : ["W+jets",     92],
    "Zjets"     : ["Z+jets",     r.TColor.kGreen-3],
    "Zjets_HFOR": ["Z+jets",     r.TColor.kGreen-3],
    "ZjetsPythia_HFOR": ["Z+jets Pythia",     r.TColor.kGreen-3],
    "zj"        : ["Z+jets",     r.TColor.kGreen-3],
    "ZjetsPythia"     : ["Z+jets",     94],
    "ZjetsPythia_HFOR": ["Z+jets",     94],
    "QCD"       : ["Multijet",   619],
    "QCDmm"     : ["Multijet",   619],
    "QCDele"    : ["Multijet (e)",   619],
    "QCDmuon"   : ["Multijet (#mu)", 617],
    "qcd"       : ["Multijet",   619],
    "ttbarW"    : ["t#bar{t}W",  r.TColor.kViolet],
    "ttbarV"    : ["t#bar{t}V",  r.TColor.kPink-4],
    "ttbarZ"    : ["t#bar{t}Z",  r.TColor.kMagenta-9],
    "ttV"       : ["t#bar{t}V",  r.TColor.kPink-4],
    "tZ"        : ["tZ",  r.TColor.kGreen+4],
    "Data"      : ["Data",  0],
    "ttbar5200" : ["t#bar{t}",          0],
    #"ttbar5200" : ["t#bar{t} MC@NLO",          0],
    "ttbar5860" : ["t#bar{t} Powheg+Herwig",   0],
    "ttbar5861" : ["t#bar{t} Powheg+Pythia",   0],
    "ttbarSherpa_PUfix" : ["t#bar{t} Sherpa",   0],
    "ttbarSherpa" : ["t#bar{t} Sherpa",   0],
    "ttbarSherpa_massless" : ["t#bar{t} Sherpa massless",   0],
    "ttbar5200_fast" : ["t#bar{t} MC@NLO fast",          0],
    "ttbar5860_fast" : ["t#bar{t} Powheg+Herwig fast",   0],
    "ttbar5861_fast" : ["t#bar{t} Powheg+Pythia fast",   0],
    "ttbar117050_fast" : ["t#bar{t} Powheg+Pythia fast", 0],
    "ttbar117050"      : ["t#bar{t} Powheg+Pythia",      0],
    "ttbar117049"      : ["t#bar{t} hadronic",      0],
    "ttH"       : ["t#bar{t}H",             2],
    "ttH125_Signal": ["t#bar{t}H",          2],
    "ttH125"       : ["t#bar{t}H",          2],
    "ttH125full"   : ["t#bar{t}H (125)",          2],
    "ttbarAlpgen"       : ["t#bar{t}",            10],
    "ttbarAlpgen-light" : ["t#bar{t}+light",      10],
    "ttbarAlpgen-HF"    : ["t#bar{t}+HF",         r.TColor.kGray],
    "ttbarAlpgen_HFOR"       : ["t#bar{t}",       10],
    "ttbarAlpgen_HFOR-light" : ["t#bar{t}+light", 10],
    "ttbarAlpgen_HFOR-HF"    : ["t#bar{t}+HF",    r.TColor.kGray],
    "ttbarAlpgen_tuned_HFOR-light" : ["t#bar{t}+light tuned", 10],
    "ttbarAlpgen_tuned_HFOR-HF"    : ["t#bar{t}+HF tuned",    r.TColor.kGray],
    "ttbarAlpgen_HFOR-HFcc"    : ["t#bar{t}+cc",    r.TColor.kGray+1],
    "ttbarAlpgen_HFOR-HFbb"    : ["t#bar{t}+bb",    r.TColor.kGray+3],
    "ttbarAlpgen_HFOR-HFcc_me"    : ["t#bar{t}+cc ME",    r.TColor.kGray+1],
    "ttbarAlpgen_HFOR-HFbb_me"    : ["t#bar{t}+bb ME",    r.TColor.kGray+3],
    "ttbarAlpgen_HFOR-HFcc_ps"    : ["t#bar{t}+cc PS",    r.TColor.kGray+2],
    "ttbarAlpgen_HFOR-HFbb_ps"    : ["t#bar{t}+bb PS",    r.TColor.kGray+4],
    "ttbar117050-ttlbbb"    : ["t#bar{t}+light (bbb)",    30],
    "ttbar117050-ttlbbc"    : ["t#bar{t}+light (bbc)",    41],
    "ttbar117050-ttlbbl"    : ["t#bar{t}+light (bbl)",    42],
    "ttbar117050-ttlother"    : ["t#bar{t}+light (other)",    0],
    "ttbar117050-ttl"    : ["t#bar{t}+light jets",    0],
    "ttbar117050-ttcc"    : ["t#bar{t}+cc",    17],
    "ttbar117050-ttbb"    : ["t#bar{t}+bb",    15],
    "ttbar117050_BCH"    : ["t#bar{t} BCH",    15],
    "ttbarMadgraph-ttl"    : ["t#bar{t}+light",    0],
    "ttbarMadgraph-ttcc"    : ["t#bar{t}+cc",    17],
    "ttbarMadgraph-ttbb"    : ["t#bar{t}+bb",    15],
    "ttbarMadgraph-ttl-tagbbb"    : ["t#bar{t}+light (bbb)",    0],
    "ttbarMadgraph-ttl-tagbbc"    : ["t#bar{t}+light (bbc)",    0],
    "ttbarMadgraph-ttl-tagbbl"    : ["t#bar{t}+light (bbl)",    0],
    "ttbarMadgraph-ttl-tagother"    : ["t#bar{t}+light (other)",    0],
    "ttbarMadgraph-ttlbbb"    : ["t#bar{t}+light (bbb)",    0],
    "ttbarMadgraph-ttlbbc"    : ["t#bar{t}+light (bbc)",    0],
    "ttbarMadgraph-ttlbbl"    : ["t#bar{t}+light (bbl)",    0],
    "ttbarMadgraph-ttlother"    : ["t#bar{t}+light (other)",    0],
    "ttbar-light"    : ["t#bar{t}+light jets",    0],
    "ttbar-cc"    : ["t#bar{t}+cc",    17],
    "ttbar-bb"    : ["t#bar{t}+bb",    15],
    "scaled_ttbarAlpgen_HFOR"       : ["t#bar{t} (corr)",       10],
    "scaled_ttbarAlpgen_HFOR-light" : ["t#bar{t}+light (corr)", 10],
    "scaled_ttbarAlpgen_HFOR-HF"    : ["t#bar{t}+HF (corr)",    r.TColor.kGray],
    "scaled_ttbarAlpgen_HFOR-HFcc"    : ["t#bar{t}+cc (corr)",    r.TColor.kGray+1],
    "scaled_ttbarAlpgen_HFOR-HFbb"    : ["t#bar{t}+bb (corr)",    r.TColor.kGray+3],
    "ttbar-Light" : ["t#bar{t}+light",      10],
    "ttbar-light" : ["t#bar{t}+light jets",      10],
    "ttj"         : ["t#bar{t}+light",      10],
    "ttbar-HF"    : ["t#bar{t}+HF",         r.TColor.kGray],
    "tthf"        : ["t#bar{t}+HF",         r.TColor.kGray],
    "ttbar"       : ["t#bar{t}",            10],
    "singleTop_schan"   : ["Single top",          r.TColor.kCyan-9],
    "singleTop_Wt"      : ["Single top",          r.TColor.kAzure-5],
    "VLT_600_test"      : ["VLT (600)",           2],
    "VLB_600_test"      : ["VLB (600)",           2],
    "VLT_600_test_fast" : ["VLT (600)",           2],
    "VLB_600_test_fast" : ["VLB (600)",           2],
    "ttbar5860_fast"    : ["t#bar{t} PowHeg+Herwig",0],
    "ttbar5861_fast"    : ["t#bar{t} PowHeg+Pythia",0],
    "ttH125-bb_Signal": ["t#bar{t}H, H#rightarrowb#bar{b} (125)",          2],
    "ttH125-WW_Signal": ["t#bar{t}H, H#rightarrowWW (125)",          r.TColor.kYellow],
    "ttH125-tautau_Signal": ["t#bar{t}H, H#rightarrow#tau#bar{#tau} (125)",          r.TColor.kBlue],
    "ttH125-gg_Signal": ["t#bar{t}H, H#rightarrowgg (125)",          r.TColor.kSpring],
    "ttH125-cc_Signal": ["t#bar{t}H, H#rightarrowc#bar{c} (125)",          r.TColor.kViolet],
    "ttH125-ZZ_Signal": ["t#bar{t}H, H#rightarrowZZ (125)",          r.TColor.kOrange-3],
    "ttH125-gamgam_Signal": ["t#bar{t}H, H#rightarrow#gamma#gamma (125)",          r.TColor.kCyan-10],
    "vlt"               : ["T#bar{T} (600) Chiral",           2],
    "Hplus_180"               : ["H+ (180)",           2],
    "vlt2NDLINE"        : ["Chiral",           2],
    "singletVLT_400_H125"      : ["T#bar{T} singlet (400)",           2],
    "singletVLT_600_H125"      : ["T#bar{T} singlet (600)",           2],
    "singletVLT_700_H125"      : ["T#bar{T} singlet (700)",           2],
    "singletVLT_700"      : ["T#bar{T} singlet (700)",           2],
    "singletVLT_800_H125"      : ["T#bar{T} singlet (800)",           2],
    "doubletVLT_400_H125"      : ["T#bar{T} doublet (400)",           2],
    "doubletVLT_600_H125"      : ["T#bar{T} doublet (600)",           2],
    "doubletVLT_700_H125"      : ["T#bar{T} doublet (700)",           2],
    "doubletVLT_800_H125"      : ["T#bar{T} doublet (800)",           2],
    "BRStop2_500_Stop1_200_LSP_20_LSPt0.0_Ht0.0_Zt1.0"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(20) BR(#tilde{t_{1}}Z)=1",     2],
    "BRStop2_500_Stop1_200_LSP_20_LSPt0.0_Ht1.0_Zt0.0"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(20) BR(#tilde{t_{1}}H)=1",     2],
    "BRStop2_500_Stop1_200_LSP_20_LSPt1.0_Ht0.0_Zt0.0"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(20) BR(#tilde{#chi_{1}}t)=1",     2],
    "Stop2_500_Stop1_200_LSP_20_LSPt0.0_Ht0.0_Zt1.0"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(20) BR(#tilde{t_{1}}Z)=1",     2],
    "Stop2_500_Stop1_200_LSP_20_LSPt0.0_Ht1.0_Zt0.0"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(20) BR(#tilde{t_{1}}H)=1",     2],
    "Stop2_500_Stop1_200_LSP_20_LSPt1.0_Ht0.0_Zt0.0"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(20) BR(#tilde{#chi_{1}}t)=1",     2],
    "VLT_Stop2_500_Stop1_300_LSP_120_LSPt1.0_Ht0.0_Zt0.0"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(120) BR(#tilde{#chi_{1}}t)=1",     2],
    "Stop2_500_Stop1_300_LSP_120"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(120)",     2],
    "Stop2_500_Stop1_300_LSP_120_BrH1" : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(120)",     2],
    "Stop1_300_LSP_120"                : ["#tilde{t_{1}}(300) #tilde{#chi_{1}}(120)",     r.TColor.kRed+2],
    "Stop1300_LSP120"                : ["#tilde{t_{1}}(300) #tilde{#chi_{1}}(120)",     r.TColor.kRed+2],
    "Stop2_500_Stop1_200_LSP_20"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(20)",     2],
    "Stop2_350_Stop1_200_LSP_20"      : ["#tilde{t_{2}}(350) #tilde{#chi_{1}}(20)",     2],
    "Stop2_550_Stop1_200_LSP_20"      : ["#tilde{t_{2}}(550) #tilde{#chi_{1}}(20)",     2],
    "VLT_Stop2_500_Stop1_300_LSP_120"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(120)",     2],
    "VLT_700_H125"      : ["T#bar{T} (700)",           2],
    "VLT_700"      : ["T#bar{T} (700)",           2],
    "VLT_750_H125"      : ["T#bar{T} (750)",           2],
    "VLT_600_H125"      : ["T#bar{T} (600)",           2],
    "chiral_VLT_600_H125":["T#bar{T} (600) Chiral",           2],
    "singlet_VLT_600_H125":["T#bar{T} (600) Singlet",           2],
    "singletVLT_600":["T#bar{T} (600) Singlet",           2],
    "doublet_VLT_600_H125":["T#bar{T} (600) Doublet",           2],
    "chiral_VLT_600_H1252NDLINE":["Chiral",           2],
    "singlet_VLT_600_H1252NDLINE":["VLQ Singlet",           2],
    "doublet_VLT_600_H1252NDLINE":["VLQ Doublet",           2],
    "VLT_600_test"      : ["T#bar{T} (600)",           2],
    "VLB_600_test"      : ["B#bar{B} (600)",           2],
    "VLT_600_test_fast" : ["T#bar{T} (600)",           2],
    "VLB_600_test_fast" : ["B#bar{B} (600)",           2],
    "altttbarAlpgen_HFOR": ["Tot bkg w/ Alpgen", 0],
    "altttbar117050": ["Tot bkg w/ PowPy", 0],
    "altttbar117050_fast": ["Tot bkg w/ PowPy", 0],
    "altttbar5860_fast": ["Tot bkg w/ PowHer", 0],
    "altttbar5861_fast": ["Tot bkg w/ PowPy", 0],
    "altttbarPatchwork": ["t#bar{t} slices", 0],
    "altNoSherpa_ttbar117050-ttbb": ["No NLO rw", 0],
    }
sampleLegendAndColors = {
    "smBkg" : ["Non-t#bar{t}", r.TColor.kYellow],
    "ttbarV"    : ["t#bar{t}V",  r.TColor.kGreen-6],
    "Data"      : ["Data",  0],
    "ttH125"       : ["t#bar{t}H",          r.TColor.kGreen+2],
    "ttbar-cc"  : ["t#bar{t}+c#bar{c}"   ,r.TColor.kBlue-10],
    "ttbar-bb"  : ["t#bar{t}+b#bar{b}"   , r.TColor.kBlue-6], #-6], # kTeal+3 = Maria's proposition
    "ttbar-light" : ["t#bar{t}+light jets",      10],
    "ttbar117050-ttcc"  : ["t#bar{t}+c#bar{c}"   ,r.TColor.kBlue-10],
    "ttbar117050-ttbb"  : ["t#bar{t}+b#bar{b}"   , r.TColor.kBlue-6], #-6], # kTeal+3 = Maria's proposition
    "ttbar117050-ttl" : ["t#bar{t}+light jets",      10],
    "singletVLT_600":["T#bar{T} singlet (600)",           2],
    "UED800":["UED KK (800)",           2],
    "Bkg1":["Bkg 1",           92],
    "Bkg2":["Bkg 2",           2],
    "t4top_SM":["t#bar{t}t#bar{t} (SM)",           2],
    "t4top_BSM":["t#bar{t}t#bar{t} (EFT)",           2],
    "Sgluon_800":["Sgluon (800)",           2],
    "Stop2_500_Stop1_300_LSP_120"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(120)",     2],
    "Stop2_500_Stop1_300_LSP_120_BrH1" : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(120)",     2],
    "Stop1_300_LSP_120"                : ["#tilde{t_{1}}(300) #tilde{#chi_{1}}(120)",     r.TColor.kRed+2],
    "Stop1300_LSP120"                : ["#tilde{t_{1}}(300) #tilde{#chi_{1}}(120)",     r.TColor.kRed+2],
    "Stop2_500_Stop1_200_LSP_20"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(20)",     2],
    "Stop2_350_Stop1_200_LSP_20"      : ["#tilde{t_{2}}(350) #tilde{#chi_{1}}(20)",     2],
    "Stop2_550_Stop1_200_LSP_20"      : ["#tilde{t_{2}}(550) #tilde{#chi_{1}}(20)",     2],
    "VLT_Stop2_500_Stop1_300_LSP_120"      : ["#tilde{t_{2}}(500) #tilde{#chi_{1}}(120)",     2],
    }



## 8 TeV analysis
#thesamplesTTH = ["QCD","singleTop", "Dibosons","Zjets", "Wjets","ttbarV","ttbar-HF","ttbar-light","ttH125","Data"]
#thesamplesTTH = ["QCDmm","singleTop", "Dibosons","ZjetsPythia_HFOR", "WjetsPythia_HFOR","ttbarAlpgen_tuned_HFOR-HF","ttbarAlpgen_tuned_HFOR-light","Data"]
#thesamplesTTH = ["QCDmm","singleTop", "Dibosons","Zjets_HFOR", "WjetsPythia_HFOR","ttbarAlpgen_HFOR-HFbb","ttbarAlpgen_HFOR-HFcc","ttbarAlpgen_HFOR-light","ttH125_Signal","Data"]
thesamplesTTH = ["QCDmm","singleTop", "Dibosons","ZjetsPythia_HFOR", "WjetsPythiaAuto_HFOR","ttbarV","ttbar117050-ttbb","ttbar117050-ttcc","ttbar117050-ttl","ttH125","Data"]
thesamplesTTH = ["QCDmm","singleTop", "Dibosons","Zjets", "Wjets","ttbarV","ttbar-bb","ttbar-cc","ttbar-light","ttH125","Data"]
thesamplesTTH_ftm = ["smBkg","ttbarV","ttbar117050-ttbb","ttbar117050-ttcc","ttbar117050-ttl","ttH125","Data"]
#thesamplesTTH = ["QCDmm","singleTop", "Dibosons","Zjets_HFOR", "WjetsPythiaAuto_HFOR","ttbarV","ttbar117050-ttbb","ttbar117050-ttcc","ttbar117050-ttl","ttH125","Data"]
#thesignalTTH = "VLT_700_H125"
# prefit / postfit
thesignalTTH = "ttH125"

# thevariablesTTH = ["Njets20", "JetPt1", "LepPt", "LepEta", "MET", "Wlep_MassT", "HTHad", "Nbjets","HTAll"]

thevariablesTTH = ["HTHad", "Njets25", "MET", \
                   "JetPt1", "JetPt2", "JetPt3", "JetPt4",\
                   "JetEta1", "JetEta2", "JetEta3", "JetEta4",\
                   "LepPt", "LepEta",\
#                   "DiscreteBtagW1", "DiscreteBtagW2", "DiscreteBtagW3", "DiscreteBtagW4",\
                   "Wlep_MassT", "Nbjets", "HTAll","Wlep_pt","Whad_pT", "deltaPhi_lep_MET",
                   "Mu","NPV",
                   "Mu","NPV",
#                   "MV1o_JetPt1","MV1o_JetPt2","MV1o_JetPt3","MV1o_JetPt4",
#                   "MV1o_JetEta1","MV1o_JetEta2","MV1o_JetEta3","MV1o_JetEta4",
#                   "MV1o_DiscreteBtagW1","MV1o_DiscreteBtagW2","MV1o_DiscreteBtagW3","MV1o_DiscreteBtagW4",
#                   "LepD0sig","MuEtcone20","MuPtcone30","MuMiniIso"
                  ]
#thevariablesTTH = ["HTAll","HTHad","Njets25","MET","JetPt1","LepPt","LepEta","Wlep_MassT","Nbjets","deltaPhi_lep_MET","Wlep_pt"]
thevariablesTTH = ["HTAll","HTHad","Njets25","MET","JetPt1","JetPt2","JetPt3","JetPt4","JetEta1","JetEta2","LepPt","LepEta","Wlep_MassT","Nbjets","deltaPhi_lep_MET","Wlep_pt"]
#thevariablesTTH = ["LepD0","LepD0sig","LepZ0","LepZ0sig"]
thevariablesTTH = ["NN125_6j_ge4b_kin"]

                   # "JetBtagW1", "JetBtagW2", "JetBtagW3", "JetBtagW4",\
#                    "JetVF1", "JetVF2", "JetVF3", "JetVF4",\
#                    "Wlep_MassT", "Nbjets", "HTAll", \
#                    "SumTagBin","ProdTagBin"]

forbiddenTTH=[]
# legXYTTH=[0.53, 0.35, 0.94, 0.92] # with ATLAS labels
#legXYTTH=[0.54, 0.41, 0.94, 0.93] # with ATLAS labels but no yields
#legXYTTH=[0.5, 0.45, 0.92, 0.94] # plots
legXYTTH=[0.62, 0.55, 0.94, 0.94]

#thesamples_tprime += ["ttbar117049" ]
thesamples_tprime = ["smBkg","ttH125","ttbarV","ttbar-bb","ttbar-cc","ttbar-light","singletVLT_600","Data"]
thesignal_tprime = "singletVLT_600"
thesamples_tprime = ["Bkg2","Bkg1","Data"]
thesignal_tprime = "UED800"
#thevariables_tprime = ["HTHad", "Njets25", "MET", \
#                   "JetPt1", "JetEta1", \
#                   "JetPt2", "JetEta2", \
#                   "JetPt3", \
#                   "JetPt4", \
#                   "LepPt", "LepEta",\
#                   #"JetBtagW1", "JetBtagW2", "JetBtagW3", "JetBtagW4",\
#                   #"DiscreteBtagW1", "DiscreteBtagW2", "DiscreteBtagW3", "DiscreteBtagW4",\
#                   "Wlep_MassT", "Nbjets", "HTAll",
#                   "deltaPhi_lep_MET","Wlep_pt",
#                   "JetPtB1","JetPtB2","JetPtB3","JetPtB4","mbb_minDR",
#                    ]
thevariables_tprime = ["HTj", "jet_n", "num_jet_40", "met", \
                   "bjet1_pt", "bjet1_eta", "jet1_charge", "jet1_jvtxf", "jet1_twob", "jet1_pt", "jet1_eta", \
                   "bjet2_pt", "bjet2_eta", "jet2_charge", "jet2_jvtxf", "jet2_twob", "jet2_pt", "jet2_eta", \
                   "bjet3_pt", "bjet3_eta", "jet3_charge", "jet3_jvtxf", "jet3_twob", "jet3_pt", "jet3_eta", \
                   "bjet4_pt", "bjet4_eta", "jet4_charge", "jet4_jvtxf", "jet4_twob", "jet4_pt", "jet4_eta", \
                   "bjet5_pt", "bjet5_eta", "jet5_charge", "jet5_jvtxf", "jet5_twob", "jet5_pt", "jet5_eta", \
                   "bjet6_pt", "bjet6_eta", "jet6_charge", "jet6_jvtxf", "jet6_twob", "jet6_pt", "jet6_eta", \
                   "lep_pt", "lep_eta", "lep_phi", \
                   "sum_charge","sum_charge_b","sum_charge_u","sum_charge_u_lepsplit",
                   #"JetBtagW1", "JetBtagW2", "JetBtagW3", "JetBtagW4",\
                   #"DiscreteBtagW1", "DiscreteBtagW2", "DiscreteBtagW3", "DiscreteBtagW4",\
                   "WlepMT", "Nbjets", "HTAll",
                   "deltaPhi_lep_MET","Wlep_pt","bjet_n",
                   "WhadM","WhadDR","WhadSpt","Mjjb","Mjbb","Mlnujj","Mjjj","mjj_mindR","mjj_maxPt","dRbb_av","dRbb_maxPt","mbb_maxPt","mbb_maxM","dRlepbb_mindR","mbj_mindR","mbj_maxPt","lep_charge",
                   "MV1o_JetPt1","MV1o_JetPt2","MV1o_JetPt3","MV1o_JetPt4",
                   "MV1o_JetEta1","MV1o_JetEta2","MV1o_JetEta3","MV1o_JetEta4",
                   ]
thevariables_tprime += [ "Hbb_candidates", "Zbb_candidates", "Zjj_candidates", "Wjj_candidates"]
thevariables_tprime += [ "MinDR_LepJ", "MinDR_LepB" ]
thevariables_tprime += [ "MinDR_bb", "MinDR_bb_Mass","MinDR_bb_Pt",]
thevariables_tprime += [ "NoMinDR_bb", "NoMinDR_bb_Mass","NoMinDR_bb_Pt",]
thevariables_tprime += [ "maxDeltaPhi_bb","DeltaPhi_b3b4","DeltaPhi_bb_minPt" ]
thevariables_tprime += [ "MV1_4_MinDR_bb", "MV1_4_MinDR_bb_Mass","MV1_4_MinDR_bb_Pt",]
thevariables_tprime += ["lep_d0sig","lep_d0","lep_z0sig","lep_z0","lep_d0signif","lep_z0signif","el_etcone20","el_ptcone30","el_miniIso","el_miniIso_pt","mu_etcone20","mu_ptcone30","mu_miniIso","mu_miniIso_pt"]
thevariables_tprime += [ "mu","pvxp_n" ]
thevariables_tprime += [ "sum_twb","sum_twb4","sum_last_twb2" ,"count_isSemilep"]
thevariables_tprime += ["WhadMuu","signed_WhadMuu","Whad_Q","Whad_Q_lepcharge", "sum_charge_leaduu_lepcharge","discrete_sum_charge_leaduu_lepcharge"]
thevariables_tprime += ["lep_nBLHits","lep_expectBLayerHit","lep_trackphi	","lep_tracktheta	","mu_nPixHits","mu_nPixHoles","mu_nPixelDeadSensors","mu_nSCTHits","mu_nSCTHoles","mu_nSCTDeadSensors","mu_nTRTHits","mu_nTRTOutliers","mu_id_qoverp","mu_me_qoverp","mu_ms_qoverp","mu_trackqoverp","mu_me_theta	","mu_ms_theta	","mu_id_theta	","mu_ms_phi	","mu_id_phi	","mu_id_z0_exPV","mu_trackd0beam","mu_eloss","mu_elosstype","mu_id_theta_exPV","mu_id_d0sig","el_tracketa","el_ptTrkClus"]

###############
thesamples_stop2 = ["QCDmm","singleTop", "Dibosons","Zjets", "Wjets","ttH125","ttbarV","ttbar-bb","ttbar-cc","ttbar-light","Stop1300_LSP120","Stop2_500_Stop1_300_LSP_120_BrH1","Data"]
thesamples_stop2 = ["smBkg","ttH125","ttbarV","ttbar-bb","ttbar-cc","ttbar-light","Stop1300_LSP120","Stop2_500_Stop1_300_LSP_120_BrH1","Data"]
#thesamples_stop2 = ["smBkg","ttH125","ttbarV","ttbar-bb","ttbar-cc","ttbar-light","Stop1_300_LSP_120","Stop2_500_Stop1_300_LSP_120_BrH1","Data"]
thesignal_stop2 = "Stop1"
################

#thevariables_tprime = [ "HTAll" ]
thesamples_tprime_MCATNLO = ["QCDmm","singleTop", "Dibosons","Zjets_HFOR", "Wjets_HFOR","ttbarV","ttbar5200","ttH125", "VLT_600_test_fast", "Data"]
forbidden_tprime=[]
legXY_tprime=[0.58, 0.7, 0.94, 0.94]
legXY_tprime_noyields=[0.64, 0.7, 0.94, 0.93]
legXY_tprimeWB_noyields=[0.56, 0.65, 0.9, 0.99]
legXY_tprimeWB_noyields_left=[0.17, 0.65, 0.47, 0.99]
legXY_tprimeWB_loose=[0.62, 0.6, 0.85, 0.99]
legXY_tprimeWB_tight=[0.17, 0.6, 0.4, 0.99]
#legXY_tprimeWB_tight=[0.17, 0.32, 0.4, 0.87]
legXY_tprime_noyields_wip=[0.555, 0.41, 0.94, 0.93]
legXY_tprime_tight=[0.5, 0.45, 0.94, 0.94]

rightATLAStext = "VLQAna_WbX_WpreselType2_M  VLQAna_WbX_1W_MWb_4"

thesamples_tprimeWB = ["QCDmm","singleTop", "Dibosons","Zjets", "Wjets","ttbarV","ttbarAlpgen_HFOR-HF","ttbarAlpgen_HFOR-light","ttH125", "singlet_VLT_600_test", "Data"]
thesignal_tprimeWB = "singlet_VLT_600_test"
thevariables_tprimeWB = ["Njets25", "HTHad", "MET", \
                   "JetPt1", "JetPt2", "JetPt3", "JetPt4",\
                   "JetEta1", "JetEta2", "JetEta3", "JetEta4",\
                   "LepPt", "LepEta",\
                   "JetBtagW1", "JetBtagW2", "JetBtagW3", "JetBtagW4",\
                   "DiscreteBtagW1", "DiscreteBtagW2", "DiscreteBtagW3", "DiscreteBtagW4",\
                   "Wlep_MassT", "Nbjets", "HTAll",
                   "Mu","NPV"]


forbidden_tprimeWB=[]
legXY_tprimeWB=[0.5, 0.45, 0.94, 0.94]
#thechannels= ["MUON_CH1HT_NOMINAL", "MUON_CH2HT_NOMINAL", "MUON_CH3HT_NOMINAL", "MUON4jet0tagex_NOMINAL", "MUON4jet1taginHT_NOMINAL", "MUON4jet1taginMjjj_NOMINAL", "MUON4jet2taginMjjj_NOMINAL", "MUON4jet2taginHT_NOMINAL"]


#lumi = 14323.9
lumi = 20340

#_________________________________________________________

def setPlotSettings(ana,channel,samples,signal,ftm):
    LEP=""
    Lep=""
    lep=""
    if "ELEMUON" in channel:
        LEP = "ELEMUON"
        lep = "elemu"
    elif "MUON" in channel:
        Lep = "Muon"
        LEP = "MUON"
        lep = "mu"
    elif "ELE" in channel:
        Lep = "Ele"
        LEP = "ELE"
        lep = "ele"
    if ana=="ttH":
        if ftm:
          thesamples=thesamplesTTH_ftm
        else:
          thesamples=thesamplesTTH
        thesignal=thesignalTTH
        #tags = int(channel[channel.find("btag")-1:channel.find("btag")])
        #jets = int(channel[channel.rfind("jet")-1:channel.rfind("jet")])
        #hist="HTHad"
        #hist="HTj"
        #if jets >= 5 and tags >= 2:
        #  hist = "NN125_%dj_%db_nob" % (jets,tags)
        #  hist = hist.replace("4b","ge4b")
        #print hist,channel
        thevariables=thevariablesTTH
        forbidden=forbiddenTTH
        legXY=legXYTTH
        if "ELEMUON" in channel:
            lep = ""
            Lep = ""
    elif "tprime" in ana:
        thesamples=thesamples_tprime
        if "MCATNLO" in ana:
            thesamples=thesamples_tprime_MCATNLO
        thesignal=thesignal_tprime
        thevariables=thevariables_tprime
        forbidden=forbidden_tprime
        legXY=legXY_tprime
    elif "stop2" in ana:
        thesamples=thesamples_stop2
        thesignal=thesignal_stop2
        thevariables=thevariables_tprime
        forbidden=forbidden_tprime
        legXY=legXY_tprime

    if len(samples) > 0:
        thesamples = samples.split(' ')
    if len(signal) > 0:
        thesignal=signal
            
    return thesamples,thesignal,thevariables,forbidden,legXY,lep,Lep,LEP
            
#__________________________________________________________

def writeYields(sample, cut, channel, myFile, verbose):
    #myFile.cd("%s/%s/%s" % (sample, channel, cut ) )
    #histo = myFile.Get("h1_HT_all_%s_%s" % (sample, cut) )
    histo = myFile.Get("%s/%s/%s/h1_n_jets_%s_%s" % (sample, channel, cut, sample, cut) )
    #histo.Reset()
    #histo.SetDirectory(0)
    if verbose:
        print type( histo )
    yi = histo.Integral(0,-1)
    yi = yi*4713.11
    return yi
#__________________________________________________________

def setCoolRange(histogram, variable):
    if "hist" in variable:
        histogram.GetXaxis().SetBinLabel(1,"Channel A")
        histogram.GetXaxis().SetBinLabel(2,"Channel B")
    if variable is "JetPt1":
        histogram.GetXaxis().SetRangeUser(0., 600.)
    if variable is "LepPt":
        histogram.GetXaxis().SetRangeUser(0., 350.)
    if variable is "MET":
        histogram.GetXaxis().SetRangeUser(0., 400.)
    if variable is "Wlep_MassT":
        histogram.GetXaxis().SetRangeUser(0., 400.)
    #if "DR" in variable:
    #    histogram.GetXaxis().SetRangeUser(0., 3.5)
#__________________________________________________________
def getOverflow(histo,variable,ana):
    if 'tprime' not in ana:
        return (0.,)
    lastNewBinVal = 0.
    if "WpreselType" in variable:
        if "_M" in variable: lastNewBinVal = 160.0
        elif "_Pt" in variable:  lastNewBinVal = 500.0
    elif variable=="JetPtB2":  lastNewBinVal = 400.0
    elif "VLQAna" in variable and "DR" in variable: lastNewBinVal = 3.6
    else:
        return (0.,)
    lastNewBin = histo.FindBin(lastNewBinVal)
    overflowCont = 0.0
    overflowErr  = 0.0
    for b in range(lastNewBin, histo.GetNbinsX()+2):
        #print "boh",b," ",lastNewBinVal
        overflowCont += histo.GetBinContent(b)
        overflowErr  += histo.GetBinError(b)**2
    #print "aaaaaaaa", overflowCont
    return (overflowCont, overflowErr)
    
def setOverflows(histo, bincon):
    if len(bincon)<2:
        return
    lastbin = histo.GetNbinsX()
    value = histo.GetBinContent(lastbin)+bincon[0]
    errorsq =  histo.GetBinError(lastbin)**2+bincon[1]
    histo.SetBinContent(lastbin, value)
    histo.SetBinError(lastbin, errorsq**0.5)
    #print histo.GetBinContent(lastbin)
    #print histo.GetBinError(lastbin)
    return

def getRebinVal(variable,ana):

    if(ana == 'tprime' or ana == 'tprimewb' or ana == 'stop2' or ana == 'ttH' ): #FIXME moved ttH to tprime binning
        grp5 = "jet1_pt bjet1_pt JetPt1 JetPtB1 JetPt2 MV1o_JetPt1 "
        grp5 += " MV1o_JetPt2 MV1o_JetPt3 MV1o_JetPt4 MV1o_JetPt5 "
        grp5 += " JetPtB2 JetPtB3 JetPtB4 JetPtB5 "
        grp5 += " lep_d0sig lep_d0 lep_z0sig lep_z0 lep_d0signif lep_z0signif el_etcone20 el_ptcone30 el_miniIso el_miniIso_pt mu_etcone20 mu_ptcone30 mu_miniIso mu_miniIso_pt"
        grp5 += " sum_charge sum_charge_b sum_charge_u sum_charge_u_lepsplit sum_charge_leaduu_lepcharge Whad_Q Whad_Q_lepcharge"
        grp5 += " el_ptTrkClus mu_eloss"
        grp3 = "JetPt3 JetPt4 JetPt5 JetPt6"
        grpspecial = "VLQAna_WbX_MinDRlb VLQAna_WbX_MinDRWb VLQAna_WbX_DRLepMet"
        grp2 = " pvxp_n jet2_pt jet3_pt jet4_pt jet5_pt MV1_4_MinDR_bb_Mass"
        grp2 += "  bjet2_pt bjet3_pt bjet4_pt bjet5_pt "
        grp2 += " MinDR_bb_Mass signed_WhadMuu"
        #grp4 = "LepPt MET"
        #grpspecial = "JetEta1 LepEta Wlep_MassT"
        #grp2 = "HTHad"
        rebF = 1
        if variable=="HTAll" or variable=="HTnolep":
            xaxis = array( 'd', [0.0, 100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0, 1000.0, 1200.0, 1400.0, 1600.0, 1800.0, 2000.0])
            return (14, "new_%s"%variable, xaxis)
        elif variable=="HTj":
            xaxis = array( 'd', [x * 50 for x in range(15)] + [800, 1000, 1200])
            return(len(xaxis)-1,"new_%s"%variable, xaxis)
        elif variable=="HTHad" or ("HT" in variable and "Had" in variable) :
            xaxis = array( 'd', [0.0, 100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0, 1000.0, 1200.0, 1500.0])#, 1800.0, 2000.0])
            #return (14, "new_%s"%variable, xaxis)
            return (11, "new_%s"%variable, xaxis)
            #xaxis = array( 'd', [0.0, 50., 100.0, 150., 200.0, 250., 300.0, 350., 400.0, 450.,  500.0, 550.,  600.0, 650., 700.0, 750.,  800.0, 850., 900., 950., 1000.0, 1050., 1100.0, 1150., 1200.0, 1250., 1300.0, 1350., 1400.0, 1450., 1500.0])
            #return (30, "new_%s"%variable, xaxis)
        elif "WpreselType" in variable:
            if "_M" in variable:
                xaxis = array( 'd', [0.0, 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 120.0, 130.0, 140.0, 150.0, 160.0])
                return (16, "new_%s"%variable, xaxis)
            elif "_Pt" in variable:
                xaxis = array( 'd', [0.0, 20.0, 40.0, 60.0, 80.0, 100.0, 120.0, 140.0, 160.0, 180.0, 200.0, 220.0, 240.0, 260.0, 280.0, 300.0, 320.0, 340.0, 360.0, 380.0, 400.0, 420.0, 440.0, 460.0, 480.0, 500.0])
                return (25, "new_%s"%variable, xaxis)
            else:
                return (rebF,)
        elif "JetPtB2" in variable or "JetPtB3" in variable or "JetPtB4" in variable:
            xaxis = array( 'd', [0.0, 20., 40., 60., 80., 100.0, 140., 200.0, 300.0, 400.0])
            return (8, "new_%s"%variable, xaxis)
        else:
            if "VLQAna_WbX_1W_MWb_" in variable:
                xaxis = array( 'd', [ 0. , 300. , 450. , 600. , 750. , 1000.])
                return ( 5 , "new_%s"%variable, xaxis)
                #rebF = 4
            elif variable in grpspecial:
                xaxis = array( 'd', [0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6])
                return (18 , "new_%s"%variable, xaxis)
            elif variable in grp5:
                rebF = 5
            elif variable in grp3:
                rebF = 3
            #elif variable in grpspecial:
            #    rebF = 2
            elif variable in grp2:
                rebF = 2
            return (rebF,)

    elif(ana == 'ttH'):
        rebF = 1
        if variable=="HTAll" or ("HT" in variable and "All" in variable) :
            xaxis = array( 'd', [0.0, 100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0, 1000.0, 1200.0, 1400.0, 1600.0, 1800.0, 2000.0])
            return (14, "new_%s"%variable, xaxis)
        elif variable=="HTj":
            xaxis = array( 'd', [x * 50 for x in range(15)] + [800, 1000, 1200])
            return(len(xaxis)-1,"new_%s"%variable, xaxis)
        if "Lep" in variable and "0" in variable: rebF = 2
#         grp3 = "HTAll"
#         grp3 = "JetPt1 LepPt Wlep_MassT MET"
#         grp2 = "JetEta1 LepEta"
#         if variable in grp3:
#             rebF = 5
#         elif variable in grp2:
#             rebF = 2
#         elif variable in grp3:
#             rebF = 4
        return (rebF,)
    else:
        rebF = 1
        return (rebF,)

#_________________________________________________________

def getError(hist):
    error=0
    for i in range(hist.GetNbinsX()+1):
        error+=hist.GetBinError(i)*hist.GetBinError(i)
        pass
    err=r.Double(0)
    print "error from cal = "+str(math.sqrt(error))+"  integral = "+str(hist.IntegralAndError(0,-1,err))+"  err = "+str(err)+"  int = "+str(hist.Integral(0, -1))
    return math.sqrt(error)

#_________________________________________________________
            
def atlasLabel(xcoord,ycoord,complt):
    # la=TLatex(); la.SetNDC(); la.SetTextFont(72); la.SetTextSize(0.045); la.DrawLatex(0.22+shift,0.2,"ATLAS")
    la=TLatex()
    la.SetNDC()#; la.SetTextFont(72); la.SetTextSize(0.045); la.DrawLatex(0.22+shift,0.2,"ATLAS")
    la.DrawLatex(xcoord,ycoord,"#font[72]{ATLAS}"+" #font[42]{"+complt+"}")
    return la

#__________________________________________________________

def setDensityBinning(hist) :
    width=hist.GetXaxis().GetBinWidth(1)
    if isWBXmreco:
        width=150.
    newhist=hist.Clone()
    newhist.SetName(hist.GetName()+"_rb")
    for bin in range(0,hist.GetNbinsX()+1) :
        if width != newhist.GetXaxis().GetBinWidth(bin) :
            NB=width/newhist.GetXaxis().GetBinWidth(bin)
            newhist.SetBinContent(bin,hist.GetBinContent(bin)*NB)
            newhist.SetBinError(bin,hist.GetBinError(bin)*NB)
            pass
        pass
    return newhist

#__________________________________________________________

if __name__=="__main__":

    parser = OptionParser()
    parser.add_option ("-f","--ftm",
                       help="Is FTM output",
                       dest="ftm",
                       action="store_true",
                       default=False)
    parser.add_option("-c","--channel",
                      help="DR region",
                      dest="channel",
                      default="MUON_CH3_NOMINAL")
    parser.add_option ("-d","--dir",
                       help="dir",
                       dest="dir",
                       default="datamc_btagOP70/")
    parser.add_option ("-v","--verbose",
                       help="Turn on verbose printout",
                       dest="verbose",
                       action="store_true",
                       default=False)
    parser.add_option ("-l","--logmode",
                       help="log mode",
                       dest="logmode",
                       default="False")
    parser.add_option ("-r","--rebin",
                       help="Unactive rebinning",
                       dest="rebin",
                       default="True")
    parser.add_option ("-B","--blindedTex",
                       help="Print blinding text: Blinded above 700 GeV",
                       dest="blindedTex",
                       default="")
    parser.add_option ("-o","--odir",
                       help="output dir",
                       dest="odir",
                       default="")
    parser.add_option ("-e","--ext",
                       help="plot's extension (default = png)",
                       dest="ext",
                       default="png")
    parser.add_option ("-n","--name",
                       help="channel name in Root latex",
                       dest="name",
                       default="")
    parser.add_option ("-t","--tdir",
                       help="directory for yield files",
                       dest="tdir",
                       default="../dataMcPlots")
    parser.add_option ("-y","--yext",
                       help="format of yield file, tex or txt",
                       dest="yext",
                       default="tex")
    parser.add_option ("-a","--analysis",
                       help="analysis: tprime or ttH",
                       dest="ana",
                       default="tprime")
    parser.add_option ("-p","--prefix",
                       help="prefix in sample names (ex: PrefitPlot)",
                       dest="pref",
                       default="")
    parser.add_option ("-u","--uncert",
                       help="Will print uncertainties from sample syste",
                       dest="uncert",
                       default="")
    parser.add_option ("-b","--label",
                       help="To write ATLAS label, possibilities: work_in_progress, \"Preliminary\" and \"\" (papers), please use \"_\" as separator",
                       dest="label",
                       default="no")
    parser.add_option ("--yields",
                       help="Put yields on the plots, default=True",
                       dest="yields",
                       default="True")
    parser.add_option ("--normsig",
                       help="True if you want to normalize your signal to the background -> e.g. for MVA output",
                       dest="normsig",
                       default="False")
    parser.add_option ("--normsig2",
                       help="True if you want to normalize your signal to the background but also represent the signal stacked",
                       dest="normsig2",
                       default="False")
    parser.add_option ("--normdata",
                       help="True if you want to normalize your MC to the data-> e.g. shapes comparisons",
                       dest="normdata",
                       default="False")
    parser.add_option ("--draw",
                       help="Draw Data and Draw Signal",
                       dest="draw",
                       default="True True")
    parser.add_option ("--noratio",
                       help="True if you want to *not* plot the ratio pad. Default is False",
                       dest="noratio",
                       default="False")
    parser.add_option ("-V","--variables",
                       help="variables",
                       dest="variables",
                       default="")
    parser.add_option ("--usePseudoData",
                       help="usePseudoData, default = False",
                       dest="usePseudoData",
                       default="False")
    parser.add_option ("--useAsimovData",
                       help="useAsimovData, default = False",
                       dest="useAsimovData",
                       default="False")
    parser.add_option ("-k","--KStest",
                       help="Do Kolmogorov test or chi2 test between data and MC (poss string = KS or chi2)",
                       dest="KStest",
                       default="")
    parser.add_option ("-s","--samples",
                       help="list of samples, in the order of desired appearance",
                       dest="samples",
                       default="")
    parser.add_option ("-S","--Signal",
                       help="signal sample",
                       dest="signal",
                       default="")
    parser.add_option ("-R","--RooStatOutput",
                       help="Histos from RooStats output",
                       dest="rooStats",
                       default="")
    parser.add_option ("--saveRatio",
                       help="Save ratios in Rootfile",
                       dest="saveRatio",
                       default="False")
    #parser.add_option ("-s","--source",
    #                   help="Origin of the inputs: FTM, HistFitter",
    #                   dest="source",
    #                   default="FTM")
    parser.add_option ("--alternatettbar",
                       help="draw MC prediction using a different ttbar sample",
                       dest="alternatettbar",
                       default="")
    parser.add_option ("--alternatesignal",
                       help="draw MC prediction using a different signal sample",
                       dest="alternatesignal",
                       default="")  
    parser.add_option ("--alternatesignal2",
                       help="draw MC prediction using a different signal sample",
                       dest="alternatesignal2",
                       default="")  

    (options, args) = parser.parse_args()
    dir = options.dir
    channel = options.channel
    verbose  = options.verbose
    logmode = eval(options.logmode)
    rebin = eval(options.rebin)
    blindedTex = options.blindedTex
    odir = options.odir
    if odir == "" : odir = dir
    r.TSystem.gSystem.Exec("mkdir -p "+odir)
    ext = options.ext.split(' ')
    channelName = options.name
    tdir = options.tdir
    if tdir == "" : tdir = odir
    r.TSystem.gSystem.Exec("mkdir -p "+tdir)
    yext = options.yext
    ana = options.ana
    pref = options.pref
    uncert = options.uncert
    label = options.label.replace("_"," ")
    yields = eval(options.yields)
    normsig = eval(options.normsig)
    normsig2 = eval(options.normsig2)
    noratio= options.noratio
    normdata = eval(options.normdata)
    variables = options.variables
    noratio = eval(options.noratio)
    draw = (options.draw).split(' ')
    drawData = eval(draw[0])
    drawSignal = eval(draw[1])
    usePseudoData = eval(options.usePseudoData)
    useAsimovData = eval(options.useAsimovData)
    KStest = options.KStest
    samples = options.samples
    signal = options.signal
    rooStats = options.rooStats
    saveRatio = eval(options.saveRatio)
    alternatettbar = (options.alternatettbar).split(' ')
    alternatesignal = options.alternatesignal
    alternatesignal2 = options.alternatesignal2
    
    if ana=="tprime" or ana == 'tprimewb' or ana == "stop2" :
        if not yields:
            legXY_tprime=legXY_tprime_noyields
            if ana == 'tprimewb':
                legXY_tprime=legXY_tprimeWB_noyields
                if variables in rightATLAStext:
                    legXY_tprime=legXY_tprimeWB_noyields_left
                if "tight" in channelName:
                    legXY_tprime=legXY_tprimeWB_tight
                if "loose" in channelName and "VLQAna_WbX_1W_MWb_4" in variables:
                    legXY_tprime=legXY_tprimeWB_loose
            if "work" in label:
                legXY_tprime=legXY_tprime_noyields_wip
    
    thesamples,thesignal,thevariables,forbidden,legXY,lep,Lep,LEP=setPlotSettings(ana,channel,samples,signal,options.ftm)
    
    if variables is not "": thevariables = variables.split(' ')

        
    if "Data" in thesamples and not drawData:
        thesamples.remove("Data")

    if thesignal in thesamples and not drawSignal:
        thesamples.remove(thesignal)
            
    if channel in forbidden:
        drawData = False

    # if "singlet" in thesignal or "doublet" in thesignal:
    #     sampleLegendAndColors[thesignal] = sampleLegendAndColors[thesignal[ thesignal.index('_')+1:]]

        #if thesignal in thesamples and not drawSignal:
        #    thesamples.remove(thesignal)
            
        if channel in forbidden:
            drawData = False

    varLabels = []
    unit = []
    thelegends = []
    thecolors = []
    is_ttH = False

    if ana=="ttH":
        is_ttH = True
        sampleLegendAndColors["ttH125"][1] = r.TColor.kRed
    else:
        pass
        #sampleLegendAndColors["ttH125"][1] = r.TColor.kRed-7
        #sampleLegendAndColors["ttH125_Signal"][1] = r.TColor.kRed-7
        #sampleLegendAndColors["ttH"][1] = r.TColor.kRed-7
    for v in thevariables:
        if v in variableLabelsAndUnits.keys():
          varLabels.append(variableLabelsAndUnits[v][0])
          unit.append(variableLabelsAndUnits[v][1])
        else:
          varLabels.append("")
          #varLabels.append(v) #FIXME
          unit.append("")
    for s in thesamples:
        thelegends.append(sampleLegendAndColors[s][0])
        thecolors.append(sampleLegendAndColors[s][1])

    if "Data" in thesamples and usePseudoData:
        thelegends[thesamples.index("Data")] = "Pseudo-data"
    if "Data" in thesamples and useAsimovData:
        thelegends[thesamples.index("Data")] = "Asimov-dataset"

    dataindex = -1
    signalindex = -1

    myFiles = []
    njets = []

    #if "6jetin3btagex" in channel or "6jetin4btagin" in channel:  # not anymore 6jetin3btagex...
#     if "6jetin4btagin" in channel and "HTHad" in thevariables[0]:
#         if len(thevariables)==1 and thevariables[0]=="HTHad": thevariables=[];varLabels=[];unit=[]
#         thevariables.append("KLFHiggsMass")
#         varLabels.append("m_{b#bar{b}}")
#         unit.append("GeV")
# #             if len(thevariables)>1:
# #                 thevariables.append("KLF_EventProba")
# #                 varLabels.append("Event probability")
# #                 unit.append("")
# #                 pass
#         pass
        
#         if "JetEta4" in thevariables:
#             if "5jet" in channel or "6jet" in channel:
#                 thevariables.append("JetPt5");  thevariables.append("JetEta5")
#                 varLabels.append("p_{T}^{5th leading jet}"); varLabels.append("#eta^{5th leading jet}")
#                 unit.append("GeV"); unit.append("")
#                 if "6jet" in channel:
#                     thevariables.append("JetPt6"); thevariables.append("JetEta6")
#                     varLabels.append("p_{T}^{6th leading jet}"); varLabels.append("#eta^{6th leading jet}")
#                     unit.append("GeV"); unit.append("")
#                     pass
#                 pass
#             pass
#         if "JetBtagW4" in thevariables:
#             if "5jet" in channel or "6jet" in channel:
#                 thevariables.append("JetBtagW5")
#                 varLabels.append("5th leading btag weight")
#                 unit.append(""); unit.append("")
#                 if "6jet" in channel:
#                     thevariables.append("JetBtagW6")
#                     varLabels.append("6th leading btag weight")
#                     unit.append("")
#                     pass
#                 pass
#             pass
#         if "DiscreteBtagW4" in thevariables:
#             if "5jet" in channel or "6jet" in channel:
#                 thevariables.append("DiscreteBtagW5")
#                 varLabels.append("5th leading btag weight")
#                 unit.append("")
#                 if "6jet" in channel:
#                     thevariables.append("DiscreteBtagW6")
#                     varLabels.append("6th leading btag weight")
#                     unit.append("")
#                     pass
#                 pass
#             pass

    ind = -1
    if rooStats != "" :
        print rooStats
        myFiles.append(r.TFile.Open(rooStats,"READ"))
        #myFiles.append(r.TFile.Open(rooStats,"READ"))
        #HACK for B only fits
        #myFiles.append(r.TFile.Open(rooStats,"READ"))
        myFiles.append(r.TFile.Open(rooStats.replace("after","before").replace("VLQ_v4update_unblind_SB/plots","VLQ_v4update_signalsonly_asimov_SB_4topfix"),"READ")) #FIXME
        if "before" in rooStats: prepost = "Pre-fit"
        else: prepost = "Post-fit"
    else :
        prepost = "Pre-fit"
        for s in thesamples:
            if "QCD" in s and ana=="ttH":
                if s == "QCD" and not os.path.isfile(basepath+dir+pref+s+"_"+channel+".root"): s="QCD"+lep
                if not os.path.isfile(basepath+dir+pref+s+"_"+channel+".root") :
                    if "ELEMUON" in channel and s == "QCDele":
                        chan2=channel.replace("ELEMUON","ELE")
                        myFiles.append(r.TFile.Open(basepath+dir+pref+"QCDmm_"+chan2+".root","READ"))
                        ind += 1                    
                    elif "ELEMUON" in channel and s == "QCDmuon":
                        chan2=channel.replace("ELEMUON","MUON")
                        myFiles.append(r.TFile.Open(basepath+dir+pref+"QCDmm_"+chan2+".root","READ"))
                        ind += 1
                        pass
                    continue
                pass
            if "Data" in s:
                if not os.path.isfile(basepath+dir+pref+s+"_"+channel+".root") :
                #if not os.path.isfile(basepath+dir+pref+s+"-posQ_"+channel+".root") :
                    s=s.replace("Data","Data"+lep)
            if "vlt" in s:
                if not os.path.isfile(basepath+dir+pref+s+"_"+channel+".root") :
                    myFiles.append(r.TFile.Open(basepath+dir+s+"_"+channel+".root","READ"))
                    continue
            myFiles.append(r.TFile.Open(basepath+dir+pref+s+"_"+channel+".root","READ"))
            #myFiles.append(r.TFile.Open(basepath+dir+pref+s+"-posQ_"+channel+".root","READ"))
            ind += 1
            if "Data" in s: dataindex = ind
            if thesignal in s: signalindex = ind
            pass
        pass
    if drawData and dataindex < 0 and rooStats=="":
        print "ERROR no data file!!"
        
    if drawSignal and signalindex < 0 and rooStats=="":
        print "ERROR no signal file!!"
            
    if rooStats == "" :
        for i in range(len(myFiles)):
            if len(thevariables) > 0:
                print myFiles[i], myFiles[i].Get(thevariables[0])
                njets.append(myFiles[i].Get(thevariables[0]).Clone("nj_%s"%thevariables[0]))
            else:
                print "ERROR! no variables!"
    else :
        for s in thesamples :
            if uncert == "SampMeth" : 
                if thesignal in s:
                  print "look for hist " + "hist_"+s + " in file "+myFiles[1].GetName()
                  print myFiles[1].Get("hist_"+s)
                  njets.append(myFiles[1].Get("hist_"+s))
                else:
                  print "look for hist " + "hist_"+s + " in file "+myFiles[0].GetName()
                  print myFiles[0].Get("hist_"+s)
                  njets.append(myFiles[0].Get("hist_"+s))
            else :
                print "look for hist " + "My_"+s + " in file "+myFiles[0].GetName()
                njets.append(myFiles[0].Get("My_"+s))
                pass

    if len(thecolors)==len(thesamples) :
        for nj in njets:
            #print thesamples[njets.index(nj)]
            nj.SetFillColor(thecolors[njets.index(nj)])
            if normsig and njets.index(nj) == thesamples.index(thesignal) :
                nj.SetFillColor(r.TColor.kWhite)
                nj.SetLineColor(thecolors[njets.index(nj)])
                nj.SetLineWidth(2)
                pass
            pass
        pass
    njsignorm=r.TH1F()
    if normsig2 :
        if "Njets25" in thevariables :
            njsignorm=njets[thesamples.index(thesignal)].Clone("njsignorm")
            pass
        pass

    try:
        if yext=="txt": outFile = open(basepath+tdir+"/yields.txt" , 'a+')
        else: outFile = open(basepath+tdir+"/latexYields_%s.tex" % channel, 'wa')
    except IOError:
        print "cannot open file"

    fsize=os.path.getsize(basepath+tdir+"/yields.txt")
    ttspeIn3 = False
    if yext=="txt" and fsize is 0:
        outFile.write("Channel")
        for s in thelegends:
            if "Data" in s: outFile.write("\t Total_bkg. \t %s"%s.replace(' ','_'))
            #elif "light" in s and thelegends.index(s) != len(thelegends)-1 :
            #    outFile.write("\t %s \t t#bar{t}+jets"%s) # only available for ttbar Alpgen_HFOR for the moment
            #elif "cc" in s : outFile.write("\t %s \t t#bar{t}+HF"%s)
            else: outFile.write("\t %s"%s.replace(' ','_'))
        if not drawData: outFile.write("\t Total_bkg. \t Data")
        outFile.write("\n")
        
    writeChannel= True
    if fsize is not 0 and yext=="txt":
        if channel in outFile.read(): writeChannel=False
        
    if yext=="txt" and writeChannel :
        outFile.write(channel)

    if yext=="tex":
        # outFile.write("\\begin{table}\\centering ")
        outFile.write(" \\tiny \\begin{tabular}{l c } \\toprule\n \\multicolumn{2}{c}{ %s} \\\\ \\midrule \n" % channel.rstrip('NOMINAL').replace('_',' '))

    
    totbg = 0.; errbkg=0.
    ttjets=0.; errttjets=0.; nbf=0
    error=r.Double(0)
    for s, nj in zip(thesamples, njets):
        if writeChannel:
            if "HFcc" in s: ttspeIn3=True            
            if "Data" in s:
                if yext=="txt":
                    outFile.write("\t %.2f $\\pm$ %.2f \t %.0f"%(totbg,math.sqrt(errbkg),nj.Integral(0,-1)))
                else:
                    outFile.write(" \\midrule \n")
                    if drawData:
                        outFile.write(" \\textbf{tot bkg} & %.2f \\\\ \n" % (totbg) )
                        outFile.write(" \\midrule \n")
            else:
                if yext=="txt":
                    if nbf==2 and not ttspeIn3 :
                        outFile.write("\t %.2f $\\pm$ %.2f"%(ttjets,math.sqrt(errttjets)))
                    if ((nbf==2 and (thesamples.index(s)==len(thesamples)-1) or (nbf==3 and (thesamples.index(s)!=len(thesamples)-1)))) and ttspeIn3 :
                        outFile.write("\t %.2f $\\pm$ %.2f"%(ttjets,math.sqrt(errttjets)))
                    outFile.write("\t %.2f $\\pm$ %.2f"%(nj.IntegralAndError(0, -1,error),error))
                else: outFile.write(" \\textbf{%s} & %.2f \\\\ \n" % (s.replace('TprimeWb_500_Signal', 'Tprime500'), nj.Integral(0, -1)) )
        if "Data" in s or thesignal in s :
            if normdata:
                #if "Data" in s : totdata=nj.Integral(0,-1)-totbg #FIXME
                if "Data" in s : totdata=nj.Integral(0,-1)
                elif s == thesignalTTH : totsig=nj.Integral(0,-1)
                pass
            continue
        totbg = totbg + nj.IntegralAndError(0, -1,error)
        errbkg += error*error
        if "ttbarAlpgen_HFOR-" in s:
            nbf += 1
            ttjets += nj.IntegralAndError(0, -1,error)
            errttjets += error*error
    if yext=="txt" and writeChannel:
        if not drawData : print "not drawdata, tot";outFile.write("\t %.2f $\\pm$ %.2f \t --"%(totbg,math.sqrt(errbkg)))
        outFile.write("\n")
    elif yext=="tex":
        outFile.write("\\bottomrule\\end{tabular}")
        # outFile.write("\\caption{\\scriptsize yields in channel %s.}\\end{table} " % channel.replace('_',' '))
    outFile.close()

    thecanvas = r.TCanvas("thecanvas", "thecanvas", 520, 600 )
    # thetoppad    = r.TPad("toppad",    "toppad",    0., 0.28, 1., 1., 0, 0, 0 )
    # thebottompad = r.TPad("bottompad", "bottompad", 0., 0., 1., 0.28, 0, 0, 0)
    thetoppad    = r.TPad("toppad",    "toppad",    0.0, 0.28, 1., 1., 0, 0, 0 )
    thebottompad = r.TPad("bottompad", "bottompad", 0.0, 0., 1., 0.28, 0, 0, 0)
    nobottompad=False

    if nobottompad:
        thetoppad    = r.TPad("toppad",    "toppad",    0.0, 1., 1., 1., 0, 0, 0 )
        thecanvas.cd()
        #thebottompad.Draw()
        #thebottompad.SetFrameBorderMode(0)
        thecanvas.cd()
    ###draw pad
        thetoppad.Draw()
        thetoppad.cd()
        thetoppad.SetTopMargin(0.05) ###margin goes to the top of the pad
        thetoppad.SetBottomMargin(0.00) ###low margin goes to the bottom
        thetoppad.SetLeftMargin(0.16)
        thetoppad.SetRightMargin(0.05)
        thetoppad.SetFrameBorderMode(0)
        

    else:
        thecanvas.cd()
    ###draw pad
        thetoppad.Draw()
        thetoppad.cd()
        thetoppad.SetTopMargin(0.05) ###margin goes to the top of the pad
        thetoppad.SetBottomMargin(0.00) ###low margin goes to the bottom
        thetoppad.SetLeftMargin(0.16)
        thetoppad.SetRightMargin(0.05)
        thetoppad.SetFrameBorderMode(0)
        thecanvas.cd()

    ###draw pad   
    if not noratio:
      thecanvas.cd()
      thebottompad.Draw()
      thebottompad.cd()
      thebottompad.SetTopMargin(0.00) ###margin goes to the top of the pad
      thebottompad.SetBottomMargin(0.37)
      thebottompad.SetLeftMargin(0.16)
      thebottompad.SetRightMargin(0.05)
      thebottompad.SetFrameBorderMode(0)

    thelegend = r.TLegend( legXY[0],legXY[1],legXY[2], legXY[3])
    thelegend.SetFillStyle(0)
    thelegend.SetLineColor(0)
    thelegend.SetBorderSize(0)
    thelegend.SetShadowColor(10)
    if yields :
        thelegend.SetNColumns(2)
        thelegend.SetTextAlign(32)
        thelegend.SetColumnSeparation(-0.1)
        pass
    # thelegend.SetTextAlign(11)
    thelegend.SetTextSize(0.042)
    if label != "no" : thelegend.SetTextSize(0.042)
    thelegend.SetTextFont(42)
    # if label != "no" : thelegend.SetHeader("#font[72]{ATLAS} #font[42]{" + label + "}")
    if ana=="tprimewb":
        #thelegend.SetHeader("#scale[1.2]{ }")
        if channelName!="":
            loosetight = r.TLatex()
            loosetight.SetNDC(1)
            loosetight.SetTextFont(72)
    else:
        if channelName !="" and label != "no" : pass
        #if channelName !="" and label != "no" : thelegend.SetHeader("#scale[1.2]{"+channelName+"}")
    
    normsig2set = False
    for s, sample, nj in reversed(zip(thelegends, thesamples, njets)):
        if ("Data" in s or "data" in s) and drawData:
            nj.SetMarkerStyle(20)
            nj.SetMarkerSize(1.2)  #0.8
            thelegend.AddEntry(nj, s, "lep")
            if yields :
              if useAsimovData: # or ("HTAll" in v and (("6 j" in channelName and "3 b" in channelName) or "4 b" in channelName)):
                thelegend.AddEntry(r.NULL, "--" , "")
              else:
                thelegend.AddEntry(r.NULL, "%.1f" % (nj.Integral(0,-1)), "")
        elif thesignal in sample:
            if drawSignal :
                if normsig : thelegend.AddEntry(nj, s, "f")
                elif normsig2 and not normsig2set:
                    njsignorm.SetFillColor(r.TColor.kWhite)
                    njsignorm.SetLineColor(thecolors[njets.index(nj)])
                    njsignorm.SetLineWidth(3)
                    thelegend.AddEntry(njsignorm, s, "f")
                else : thelegend.AddEntry(nj, s, "f")
                if yields :
                    if (normsig or normsig2): thelegend.AddEntry(r.NULL, "norm", "") #FIXME
                    else :
                        if normdata: thelegend.AddEntry(r.NULL, "%.1f" % (nj.Integral(0,-1)*totdata/totbg), "")
                        else : thelegend.AddEntry(r.NULL, "%.1f" % (nj.Integral(0,-1)), "")
                    pass
                if normsig2 and not normsig2set:
                    thelegend.AddEntry(nj, s, "f")
                    if yields : thelegend.AddEntry(r.NULL, "%.1f" % (nj.Integral(0,-1)), "")
                    normsig2set = True
            if alternatesignal != "":
                altS_file = r.TFile.Open(basepath+dir+pref+alternatesignal+"_"+channel+".root","READ")
                altS = altS_file.Get(v)
                print altS, altS_file
                altS.SetLineWidth(4)
                altS.SetLineColor(2)
                altS.SetFillColor(0)
                altS.SetFillStyle(0)
                altS.SetLineStyle(2)
                thelegend.AddEntry(altS,sampleLegendAndColors[alternatesignal][0],"l") 
                if yields : thelegend.AddEntry(r.NULL, "%.1f"  % (altS.Integral(0,-1)), "")
                #thelegend.AddEntry(r.NULL,sampleLegendAndColors[alternatesignal+"2NDLINE"][0],"") 
                #if yields : thelegend.AddEntry(r.NULL, "", "")
            pass
            if alternatesignal2 != "":
                altS2_file = r.TFile.Open(basepath+dir+pref+alternatesignal2+"_"+channel+".root","READ")
                altS2 = altS2_file.Get(v)
                altS2.SetLineWidth(4)
                altS2.SetLineColor(2)
                altS2.SetFillColor(0)
                altS2.SetFillStyle(0)
                altS2.SetLineStyle(3)
                thelegend.AddEntry(altS2,sampleLegendAndColors[alternatesignal2][0],"l") 
                if yields : thelegend.AddEntry(r.NULL, "", "")
                #thelegend.AddEntry(r.NULL,sampleLegendAndColors[alternatesignal2+"2NDLINE"][0],"") 
                #if yields : thelegend.AddEntry(r.NULL, "", "")
            pass
        else:
            thelegend.AddEntry(nj, s, "f")
            if yields :
                if normdata : thelegend.AddEntry(r.NULL, "%.1f" % (nj.Integral(0,-1)*totdata/totbg), "")
                else : thelegend.AddEntry(r.NULL, "%.1f" % (nj.Integral(0,-1)), "")
                pass
            pass
    if yields : 
        thelegend.AddEntry(r.NULL, "Tot Bkg", "")
        if normdata : thelegend.AddEntry(r.NULL, "%.1f" % (totbg*totdata/totbg), "")
        else : thelegend.AddEntry(r.NULL, "%.1f" % (totbg), "")                        
            
    for v in thevariables:
        isWBXmreco = False
        if ana=="tprimewb" and v=="VLQAna_WbX_1W_MWb_4":
            isWBXmreco = True
        rb = (1,)
        if rebin: rb = getRebinVal( v, ana )
        if v == "KLFHiggsMass" and pref=="" and rooStats=="" : rb = (2,)
        thetoppad.Clear()
        thebottompad.Clear()
        stack = r.THStack()
        # print v
        try:
            if rooStats == "" : mc = myFiles[0].Get(v).Clone("mc_%s" % v)
            else : 
                if uncert == "SampMeth" : mc = myFiles[0].Get("hist_"+thesamples[0]).Clone("mc_%s" % v)
                else : mc = myFiles.Get("My_"+thesamples[0]).Clone("mc_%s" % v)
                pass
        except:
            print v,' xxx variable not in file ',thesamples[0]
            continue
        overflowbin = getOverflow( mc, v, ana )
        mc=mc.Rebin( *rb )
        mc.Reset()
        mcunscaled = mc.Clone("mcunscaled")
        setOverflows(mc, overflowbin)
        if v not in noDensityHist : mc=setDensityBinning(mc)
        minlog=0.8
        if rooStats == "" :
            for s, f in zip(thesamples, myFiles):
                if "Data" in s:
                    continue
                if thesignal in s and not drawSignal :
                    continue
                try:
                    hist = f.Get(v)
                    print f, hist
                except:
                    print v,' yyy variable not in file ',s
                    continue
                overflowbin = getOverflow( hist, v, ana )
                hist=hist.Rebin( *rb )
                hist.SetLineWidth(2)
                #print s," prima ",hist.GetBinContent(hist.GetNbinsX())
                setOverflows(hist, overflowbin)
                #print "     dopo ",hist.GetBinContent(hist.GetNbinsX())
                if len(thecolors)==len(thesamples)  : hist.SetFillColor(thecolors[thesamples.index(s)])
                #if len(thecolors)==len(thesamples) and thesignal not in s : hist.SetFillColor(thecolors[thesamples.index(s)])
                histintegral = hist.Integral(0,-1)
                if v not in noDensityHist : hist=setDensityBinning(hist)
                if not thesignal in s:
                    mcunscaled.Add(hist)
                if normdata : hist.Scale(totdata/totbg)
                if (thesignal in s and not normsig) or thesignal not in s :
                    stack.Add(hist)
                if not thesignal in s:
                    mc.Add(hist)
                if thesignal in s and (normsig or normsig2) :
                    hsig=hist.Clone("hsig")
                    hsig.Scale(totdata/histintegral)
                    hsig.SetLineColor(thecolors[thesamples.index(s)])
                    hsig.SetLineWidth(3)
                    hsig.SetFillColor(r.TColor.kWhite)
                    pass     
        else :
            for s in thesamples :
                if "Data" in s: continue
                if thesignal in s and not drawSignal : continue
                try :
                    if uncert == "SampMeth" : 
                      if not thesignal in s:
                        hist = myFiles[0].Get("hist_"+s)
                      else:
                        hist = myFiles[1].Get("hist_"+s)
                        print "Getting before fit",s
                    else : hist = myFiles[0].Get("My_"+s)
                except :
                    print v,' zzz variable not in file ',s
                    continue
                overflowbin = getOverflow( hist, v, ana )
                hist=hist.Rebin( *rb )
                hist.SetLineWidth(1)
                hist.SetLineColor(1)
                setOverflows(hist, overflowbin)
                if len(thecolors)==len(thesamples) and thesignal not in s : hist.SetFillColor(thecolors[thesamples.index(s)])
                histintegral = hist.Integral(0,-1)
                if v not in noDensityHist : hist=setDensityBinning(hist)
                print "XXX",histintegral,hist.Integral(0,-1)
                if normdata : hist.Scale(totdata/totbg)
                if (thesignal in s and not normsig) or thesignal not in s : stack.Add(hist)
                if thesignal in s:
                    if ana == "ttH":
                      mc.Add(hist)#FIXME
                else:
                    mc.Add(hist)
                if thesignal in s and (normsig or normsig2) :
                    hsig=hist.Clone("hsig")
                    hsig.Scale(totbg/histintegral)
                    hsig.SetLineColor(thecolors[thesamples.index(s)])
                    hsig.SetLineWidth(3)
                    hsig.SetFillColor(r.TColor.kWhite)
                    pass     
                pass
            pass
        thetoppad.cd()
        x0 = mc.GetXaxis().GetXmin()
        x1 = mc.GetXaxis().GetXmax()
        nb = mc.GetNbinsX()
        dummy = r.TH1F("dummy_%s" % v, "dummy_%s" % v, nb, x0, x1)
        setCoolRange(dummy, v)
        stack.Draw("hist")
        stack.GetHistogram().SetLineColor(1)
        stack.Draw("hist")
        setCoolRange(stack, v)
        mc.SetFillColor(r.TColor.kBlue-7)
        mc.SetFillStyle(3344)
        mc.SetMarkerStyle(1)
        mc.Draw("E2,same") #Stat error
        maxi = stack.GetMaximum()
        stack.GetYaxis().SetLabelFont(43)
        stack.GetYaxis().SetLabelSize(20)
        binW = mc.GetXaxis().GetBinWidth(1)
        if binW % 1 == 0 : binW=int(binW)
        binW=str(binW)
        if isWBXmreco:
            binW="150"
        if unit[thevariables.index(v)] != "" and binW.isalpha() == False:
            stack.GetYaxis().SetTitle("Events / "+str(binW)+" "+unit[thevariables.index(v)])
        else: stack.GetYaxis().SetTitle("Events / "+str(binW))
        if str(binW)=="1":
            stack.GetYaxis().SetTitle("Events")
        stack.GetYaxis().SetTitleSize(0.05)
        stack.GetYaxis().SetTitleOffset(1.7)
        if normsig or normsig2 :
            hsig.Draw("hist,same")
            pass
        if noratio: 
          dummy = stack.GetHistogram()
          dummy.GetXaxis().SetTitleOffset(1.7)
        else:
          thebottompad.cd()
          dummy.Draw()
          setCoolRange(dummy, v)
          dummy.GetYaxis().SetLabelFont(43)
          dummy.GetYaxis().SetLabelSize(20) ###labels will be 16 pixels
          dummy.GetYaxis().SetTitleFont(43)
          dummy.GetYaxis().SetTitleSize(20) ###labels will be 16 pixels
          dummy.GetYaxis().SetTitle("    Data / Bkg  ")
          # dummy.GetYaxis().SetTitleOffset(1.3)
          dummy.GetYaxis().SetTitleOffset(2)
          dummy.GetXaxis().SetTitleOffset(3.5)
        dummy.GetXaxis().SetLabelFont(43)
        dummy.GetXaxis().SetLabelSize(20) ###labels will be 16 pixels
        dummy.GetYaxis().SetLabelFont(43)
        # dummy.GetYaxis().SetLabelSize(20) ###labels will be 16 pixels
        dummy.GetYaxis().SetLabelSize(20) ###labels will be 16 pixels
        dummy.GetXaxis().SetTitleFont(43)
        dummy.GetXaxis().SetTitleSize(25)
        if unit[thevariables.index(v)] != "":
            dummy.GetXaxis().SetTitle(varLabels[thevariables.index(v)]+" ["+unit[thevariables.index(v)]+"]")
        else: dummy.GetXaxis().SetTitle(varLabels[thevariables.index(v)])
        dummy.GetYaxis().SetTitle("    Data / Bkg  ")
        if ana=="tprimewb":
            dummy.GetYaxis().SetTitle("    Data / Bkg  ")
        # dummy.GetYaxis().SetTitleOffset(1.3)
        dummy.GetYaxis().SetTitleOffset(2.4)
        dummy.GetXaxis().SetTitleOffset(3.5)
        r.gStyle.SetStripDecimals(True)
        r.TGaxis.SetMaxDigits(5)
        #dummy.GetYaxis().SetRangeUser(0.000001,1.999999)
        dummy.GetYaxis().SetRangeUser(0.50001,1.49999)
        # dummy.GetYaxis().SetRangeUser(0.4,1.6)
        dummy.GetYaxis().SetNdivisions(504,False)
        if not options.ftm:
          dummy.GetXaxis().SetNdivisions(505)
        # dummy.GetYaxis().SetDecimals(True)
        # dummy.GetYaxis().SetNdivisions(504)
        if "nWhad" in v:
            dummy.GetXaxis().SetNdivisions(505)
        thebottompad.Update()
        thebottompad.Modified()
        ###back to toppad                        
        thetoppad.cd()         

        if uncert != "":
            if rooStats == "":
                #fi = r.TFile.Open(basepath+dir+pref+uncert+"_"+channel+".root","READ")
                fi = r.TFile.Open(basepath+dir+pref+uncert+"_"+channel.replace('_NOMINAL','')+".root","READ")  # Antonella's file
                huncert=fi.Get(v)
                print "huncert.0 ",huncert.GetBinError(huncert.GetNbinsX())
                print basepath+dir+pref+uncert+"_"+channel.replace('_NOMINAL','')+".root"
            else :
                if uncert == "SampMeth" : huncert = myFiles[0].Get("hist_TotWithErr")
                else : huncert = myFiles[0].Get("My_FitError_AfterFit")
                overflowbin = getOverflow( huncert, v, ana )
                huncert=huncert.Rebin( *rb )
                setOverflows(huncert, overflowbin)
                pass
            # huncertbot=huncert.Clone("uncertbot")
            if False:# "tprime" not in ana:
                overflowbin = getOverflow( huncert, v, ana )
                huncert=huncert.Rebin( *rb )
                setOverflows(huncert, overflowbin)
            ##strange unc band in first and last bins... manually set just for display
            elif "Wpresel" in v and ("_M" in v or "_Pt" in v):
                huncert.SetBinContent(1,mc.GetBinContent(1))
                if mc.GetBinContent(2) > 0:
                    huncert.SetBinError(1,mc.GetBinContent(1)*huncert.GetBinError(2)/mc.GetBinContent(2))
                else:
                    huncert.SetBinError(1,0.0)
            elif "DR" in v:
                lastbin = huncert.GetNbinsX()
                if "VLQAna_WbX_MinDRlb" in v:
                    lastbin = huncert.GetNbinsX()-1
                huncert.SetBinError(lastbin, mc.GetBinError(lastbin-1))
                #else:
                #    huncert.SetBinError(lastbin, 0.0)
            print "huncert.A ",huncert.GetBinError(huncert.GetNbinsX())
            if v not in noDensityHist : huncert=setDensityBinning(huncert)
            print "huncert.B ",huncert.GetBinError(huncert.GetNbinsX())
            huncert.SetFillColor(r.TColor.kBlue-7)
            huncert.SetFillStyle(3454)
            huncert.SetLineWidth(0)
            huncert.SetLineStyle(0)
            huncert.SetLineColor(0)
            #huncert.SetFillStyle(3344)
            huncert.SetMarkerStyle(1)
            for ibin in range(1,huncert.GetNbinsX()+1):
                huncert.SetBinContent(ibin,mc.GetBinContent(ibin))
            bkglegendstring = "Total Bkg unc."
            bkglegendstring2 = ""
            huncert.Draw("E2,same")
            thelegend.AddEntry(huncert, bkglegendstring, "fe")
            if yields : thelegend.AddEntry(r.NULL, "", "")
            if bkglegendstring2!="":
                thelegend.AddEntry(r.NULL, bkglegendstring2, "")
                if yields : thelegend.AddEntry(r.NULL, "", "")
            #FIXME check if huncertbot contains signal
            huncertbot=huncert.Clone("uncertbot")
            print huncertbot.Integral(), mc.Integral()
            # huncertbot=fi.Get(v).Clone("uncertbot")
            for ibin in range(1,huncertbot.GetNbinsX()+1):
                huncertbot.SetBinContent(ibin,1.0)
                if huncert.GetBinContent(ibin) != 0: huncertbot.SetBinError(ibin,huncert.GetBinError(ibin)/mc.GetBinContent(ibin))
                else: huncertbot.SetBinError(ibin,0)
                pass

            print "Max = " +str(huncert.GetMaximum()) + "  bin = " +  str(huncert.GetMaximumBin()) + "  content = " + str(huncert.GetBinContent(huncert.GetMaximumBin()))+ "avec error = " +str(huncert.GetBinContent(huncert.GetMaximumBin()) + huncert.GetBinError(huncert.GetMaximumBin()))
            maxi = huncert.GetBinContent(huncert.GetMaximumBin()) + huncert.GetBinError(huncert.GetMaximumBin())
            
            if not noratio: 
              thebottompad.cd()
              huncertbot.Draw("E2,same")
            thetoppad.cd()
            pass
        if drawData:
            if rooStats == "": data = myFiles[dataindex].Get(v)
            else : 
                if uncert == "SampMeth" : data = myFiles[0].Get("hist_Data")
                else : data = myFiles[0].Get("My_Data")
            overflowbin = getOverflow( data, v, ana )
            data=data.Rebin( *rb )
            data.SetLineWidth(2)
            data.SetLineColor(1)
            setOverflows(data, overflowbin)
            #data.Add(mcunscaled,-1) #FIXME
            #print "DATA!!",data.GetBinContent(data.GetNbinsX())
            if ( maxi < data.GetMaximum() ):
                maxi = data.GetMaximum()
            for ibin in range(0,data.GetNbinsX()+1) : 
              data.SetBinError(ibin, math.sqrt(data.GetBinContent(ibin)))
            dataC = r.TH1F("dataC","dataC",data.GetNbinsX(),data.GetXaxis().GetXmin(),data.GetXaxis().GetXmax())
            for ibin in range(0,dataC.GetNbinsX()+1) : 
                dataC.SetBinContent(ibin,int(data.GetBinContent(ibin)))
            if v not in noDensityHist : data=setDensityBinning(data)
            minlog=data.GetMinimum(0)/5.
            data.SetMarkerStyle(20)
            data.SetMarkerSize(1.2)  #0.8
            #if "HTAll" in v and (("6 j" in channelName and "3 b" in channelName) or "4 b" in channelName):
            #  for bin in range(1,data.GetNbinsX()+1):
            #    if data.GetBinLowEdge(bin) >= 800:
            #      data.SetBinContent(bin,0)
            #      data.SetBinError(bin,0)
            if uncert == "SampMeth" : 
              for ibin in range(mc.GetNbinsX()+1) : 
                mc.SetBinError(ibin,0)
            if useAsimovData:
              for ibin in range(0,data.GetNbinsX()+1) : 
                data.SetBinContent(ibin, mc.GetBinContent(ibin))
            elif blindedTex != "" and "HTnolep" in v and ("4 b" in channelName or ("3 b" in channelName and "High MTW" in channelName)):
              for bin in range(1,data.GetNbinsX()+1):
                if data.GetBinLowEdge(bin) >= 600:
                  data.SetBinContent(bin,0)
                  data.SetBinError(bin,0)
            #if "Nbjets" in v:
            #  for bin in range(1,data.GetNbinsX()+1):
            #    if data.GetBinLowEdge(bin) > 3:
            #      data.SetBinContent(bin,0)
            #      data.SetBinError(bin,0)
            data.Draw("E1, same")
            dataovermc = data.Clone("dataovermc_%s" % v)
            dataovermc.Divide(mc)
            dataovermc.SetMarkerStyle(20)
            dataovermc.SetMarkerSize(.8)
            dataovermc.SetLineWidth(2)
            mcovermc = mc.Clone("mcovermc_%s" % v)
            mcovermc.Divide(mc)
            for bin in range(1,mc.GetNbinsX()+1):
              mcovermc.SetBinError(bin,mc.GetBinError(bin)/mc.GetBinContent(bin))
              dataovermc.SetBinError(bin,data.GetBinError(bin)/data.GetBinContent(bin))
            if not noratio: 
              thebottompad.cd()
              dataovermc.Draw("E1, same")
              mcovermc.Draw("E2,same")
            if not logmode and saveRatio :
                oratio = r.TFile.Open(basepath+odir+"/ratio_"+channel+".root","UPDATE")
                dataovermc.Write()
                oratio.Close()
                pass
            # dataovermc.SaveAs(basepath+odir+"/ratio"+v+channel+".root")
            outofrange = r.TLine()
            outofrange.SetLineWidth(2)
            width = dataovermc.GetBinWidth(1)/10.
            for ibin in range(dataovermc.GetNbinsX()+1):
                if dataovermc.GetBinContent(ibin) >= 1.5 and dataovermc.GetBinContent(ibin)-dataovermc.GetBinError(ibin) < 1.5 and  dataovermc.GetBinContent(ibin)-dataovermc.GetBinError(ibin) >0.5:
                    outofrange.DrawLine(dataovermc.GetBinCenter(ibin),dataovermc.GetBinContent(ibin)-dataovermc.GetBinError(ibin),dataovermc.GetBinCenter(ibin),1.5)
                    outofrange.DrawLine(dataovermc.GetBinCenter(ibin)-width,dataovermc.GetBinContent(ibin)-dataovermc.GetBinError(ibin),dataovermc.GetBinCenter(ibin)+width,dataovermc.GetBinContent(ibin)-dataovermc.GetBinError(ibin))
                if dataovermc.GetBinContent(ibin) <= 0.5 and dataovermc.GetBinContent(ibin)+dataovermc.GetBinError(ibin) > 0.5 and  dataovermc.GetBinContent(ibin)+dataovermc.GetBinError(ibin) < 1.5:
                    outofrange.DrawLine(dataovermc.GetBinCenter(ibin),dataovermc.GetBinContent(ibin)+dataovermc.GetBinError(ibin),dataovermc.GetBinCenter(ibin),0.5)
                    outofrange.DrawLine(dataovermc.GetBinCenter(ibin)-width,dataovermc.GetBinContent(ibin)+dataovermc.GetBinError(ibin),dataovermc.GetBinCenter(ibin)+width,dataovermc.GetBinContent(ibin)+dataovermc.GetBinError(ibin))
                
            y0 = dataovermc.GetXaxis().GetXmin()
            y1 = dataovermc.GetXaxis().GetXmax()
            cline = r.TF1("cline","1",y0,y1)
            cline.SetLineStyle(2)
            cline.SetLineWidth(1)
            cline.SetLineColor(r.TColor.kRed)
            cline.Draw("same")
            if KStest != "" and v not in noLogVariables :
                KSlab = r.TLatex()
                KSlab.SetNDC(1)
                KSlab.SetTextFont(42)
                KSlab.SetTextSize(0.1)
                if KStest == "KS" : KSlab.DrawLatex(0.6,0.9,"KS = %.2f" %(data.KolmogorovTest(mc)))
                if KStest == "chi2" : KSlab.DrawLatex(0.6,0.9,"#chi^{2} = %.2f" %(data.Chi2Test(mc,"WWCHI2")))
                pass
            thebottompad.Update()
            thebottompad.Modified()
            ###back to toppad                        
            thetoppad.cd()          

        if alternatesignal!="":
            overflowbin = getOverflow( altS, v, ana )
            print "altS",altS.GetNbinsX()
            altS = altS.Rebin( *rb )
            setOverflows(altS, overflowbin)
            if v not in noDensityHist : altS=setDensityBinning(altS)
            thetoppad.cd()
            if normsig:
              altS.SetFillColor(r.TColor.kWhite)
              altS.Scale(totbg/altS.Integral(0,-1))
            else:
              altS.Add(mc)
            print "altS.Integral:",altS.Integral()
            altS.Draw("hist, same")
        if alternatesignal2!="":
            overflowbin = getOverflow( altS2, v, ana )
            altS2 = altS2.Rebin( *rb )
            setOverflows(altS2, overflowbin)
            if v not in noDensityHist : altS2=setDensityBinning(altS2)
            thetoppad.cd()
            if normsig:
              altS2.SetFillColor(r.TColor.kWhite)
              altS2.Scale(totbg/altS2.Integral(0,-1))
            else:
              altS2.Add(mc)
            altS2.Draw("hist, same")

        if alternatettbar[0] != "":
            mcalt = []
            dataovermcalt = []
            for t in range(len(alternatettbar)):
                mcalt.append(mc.Clone("alternateMC"+str(t)))
                for key, nj in zip(thesamples, njets):
                    if "ttbar117050" in key and "ttbarV" not in key:
                        altTT_file = r.TFile.Open(basepath+dir+pref+alternatettbar[t]+"_"+channel+".root","READ")
                        print altTT_file 
                        altTT = altTT_file.Get(v)
                        altInt = altTT.Integral(1,-1)
                        usualInt = nj.Integral(1,-1)
                        overflowbin = getOverflow( altTT, v, ana )
                        altTT = altTT.Rebin( *rb )
                        setOverflows(altTT, overflowbin)
                        usualTT_file = myFiles[thesamples.index(key)]
                        usualTT = usualTT_file.Get(v)
                        if len(rb)>1:
                            overflowbin = getOverflow( altTT, v, ana )
                            usualTT = usualTT.Rebin( *rb )
                            setOverflows(altTT, overflowbin)
                        if v not in noDensityHist :
                            #mcalt[t]=setDensityBinning(mcalt[t])
                            usualTT = setDensityBinning(usualTT)
                            altTT   = setDensityBinning(altTT)
                        if normdata : mcalt[t].Scale(totdata/(totbg-usualTT.Integral(1,-1)+altInt))
                        mcalt[t].Add(usualTT,-1)
                        mcalt[t].Add(altTT)
                    #print "usualTT ", usualTT.GetNbinsX()
                    #print "altTT ", altTT.GetNbinsX()
                dataovermcalt.append(data.Clone("dataovermcalt_%s%d" % (v,t)))
                dataovermcalt[t].Divide(mcalt[t])
                dataovermcalt[t].SetMarkerStyle(21+t)
                dataovermcalt[t].SetMarkerColor(2+t)
                dataovermcalt[t].SetMarkerSize(.8)
                dataovermcalt[t].SetLineWidth(2)
                dataovermcalt[t].SetLineColor(2+t)
                thebottompad.cd()
                dataovermcalt[t].Draw("E1, same")
                thetoppad.cd()
                mcalt[t].SetLineWidth(4)
                mcalt[t].SetLineColor(2+t)
                mcalt[t].SetFillColor(0)
                mcalt[t].SetFillStyle(0)
                mcalt[t].SetLineStyle(2+t)
                mcalt[t].SetMarkerStyle(21+t)
                mcalt[t].SetMarkerColor(2+t)
                mcalt[t].SetMarkerSize(.8)
                mcalt[t].Draw("hist, same")
                if v == thevariables[0]:
                  thelegend.AddEntry(mcalt[t],sampleLegendAndColors[alternatettbar[t]][0],"lep") 
                  if yields : thelegend.AddEntry(r.NULL, "%.1f" % (totbg-usualInt+altInt), "")


        if ("WpreselType" in v or "nWhad" in v or "HTAll" in v) and (ana=="tprimewb"):
            #hsig50x = njets[thesamples.index(thesignal)].Clone("signal50x")
            hsig50x = myFiles[thesamples.index(thesignal)].Get(v).Clone("signal50x")
            overflowbin = getOverflow( hsig50x, v, ana )
            hsig50x = hsig50x.Rebin( *rb )
            setOverflows(hsig50x, overflowbin)
            if v not in noDensityHist : hsig50x=setDensityBinning(hsig50x)
            hsig50x.Scale(50)
            hsig50x.SetLineWidth(4)
            hsig50x.SetLineColor(2)
            hsig50x.SetLineStyle(2)
            hsig50x.SetFillColor(0)
            #hsig50x.SetFillColor(2)
            #hsig50x.SetFillStyle(3344)
            thelegend.AddEntry(hsig50x,sampleLegendAndColors[thesignal][0]+" x 50","l")
            thetoppad.cd()
            hsig50x.Draw("histo same")
            
        thelegend.Draw()
        if ana=="tprimewb" and "VLQAna_WbX_1W_MWb_" in v:
            if channelName=="loose":
                loosetight.DrawLatex(0.68, 0.53,"#scale[1.2]{"+channelName+"}")
            else:
                loosetight.DrawLatex(0.23, 0.53,"#scale[1.2]{"+channelName+"}")

        prepost = channel.replace("AB_","").capitalize()
        lumiTex = r.TLatex()
        lumiTex.SetNDC(1)
        lumiTex.SetTextFont(42)
        lumiTex.DrawLatex(0.20, 0.87,"#font[42]{J. Montejo, PhD thesis}")
        lumiTex.DrawLatex(0.20, 0.78,"#font[42]{"+channelName+"}" )
        lumiTex.DrawLatex(0.20, 0.69,"#font[42]{"+prepost+"}" )
        if blindedTex != "":
            blindTex = r.TLatex()
            blindTex.SetNDC(1)
            blindTex.SetTextFont(42)
            blindTex.DrawLatex(legXY[0],legXY[1]/2, "#scale[0.92]{"+blindedTex+"}")
        #stack.SetMinimum(min(0,data.GetMinimum()))
        maxi = data.GetMaximum()
        stack.SetMaximum(1.55*(maxi+math.sqrt(maxi)))
        if channelName=="loose":
            print "maxi = ",str(maxi)," set to ",str(1.02*(maxi+math.sqrt(maxi)))
            stack.SetMaximum(150.)
        r.gStyle.SetOptStat(0)
        thetoppad.Update()
        thetoppad.Modified()

        thebottompad.cd()
        thebottompad.Update()
        thebottompad.Modified()
        thecanvas.cd()
        if not logmode :
            # use a PaveLabel
            origTopXaxis=r.TPaveLabel(0.10,0.26,0.155,0.3,"    0")
            origTopXaxis.SetFillColor(r.TColor.kWhite)
            origTopXaxis.SetFillStyle(1001)
            origTopXaxis.SetBorderSize(0)
            origTopXaxis.SetTextFont(42)
            origTopXaxis.SetTextSize(0.8)
            origTopXaxis.Draw()
            # origTopXaxis=r.TLatex()
            # origTopXaxis.SetNDC()
            # origTopXaxis.DrawLatex(0.08,0.27,"#font[42]{#scale[0.74]{   0}}")
            # origTopXaxis.DrawLatex(0.116,0.27,"#font[42]{#scale[0.74]{0}}")
            pass
        thecanvas.Update()
        thecanvas.Modified()
        for exx in ext:
            thetoppad.SetLogy(0)
            thecanvas.SaveAs(basepath+odir+"/"+v+"_"+channel+"."+exx)
            print basepath+odir+"/"+v+"_"+channel+"."+exx
            #thetoppad.SetLogy(1)
            #logstr = "_logscale"
            #thecanvas.SaveAs(basepath+odir+"/"+v+"_"+channel+logstr+"."+exx)

    for f in myFiles:
      try:  f.Close()
      except ReferenceError: continue
