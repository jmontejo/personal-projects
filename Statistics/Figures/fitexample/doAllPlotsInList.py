import os, sys


channelList=["AB_prefit","AB_postfit"]


#__________________________________________________________

if __name__=="__main__":

    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option ("-f","--ftm",
                       help="Is FTM output",
                       dest="ftm",
                       action="store_true",
                       default=False)
    parser.add_option ("-d","--dir",
                       help="dir",
                       dest="fdir",
                       default="./")
    parser.add_option ("-o","--odir",
                       help="odir",
                       dest="odir",
                       default="finalplots")
    parser.add_option ("-r","--rebin",
                       help="Unactive rebinning",
                       dest="rebin",
                       default=True)
    parser.add_option ("-e","--ext",
                       help="plot's extension (default = png)",
                       dest="ext",
                       default="png")
    parser.add_option ("-p","--prefix",
                       help="prefix in sample names (ex: PrefitPlot)",
                       dest="pref",
                       default="")
    parser.add_option ("-u","--uncert",
                       help="Will print uncertainties from sample syste",
                       dest="uncert",
                       default="")
    parser.add_option ("-b","--label",
                       help="To write ATLAS label, possibilities: \"work in progress\", \"Preliminary\" and \"\" (papers)",
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
                       help="True if you want to normalize your signal to the background and to represent the signal stacked",
                       dest="normsig2",
                       default="False")
    parser.add_option ("--noratio",
                       help="True if you want to *not* plot the ratio pad. Default is False",
                       dest="noratio",
                       default="False")
    parser.add_option ("--normdata",
                       help="True if you want to normalize your MC to the data -> e.g. shapes comparisons",
                       dest="normdata",
                       default="False")
    parser.add_option ("-v","--verbose",
                       help="Turn on verbose printout",
                       dest="verbose",
                       action="store_true",
                       default=False)
    parser.add_option ("--draw",
                       help="Draw Data and Draw Signal",
                       dest="draw",
                       default="True True")
    parser.add_option ("--useAsimovData",
                       help="useAsimovData, default = False",
                       dest="useAsimovData",
                       default="False")
    parser.add_option ("--usePseudoData",
                       help="usePseudoData, default = False",
                       dest="usePseudoData",
                       default="False")
    parser.add_option ("-k","--KStest",
                       help="Do Kolmogorov test or chi2 test between data and MC (poss string = KS or chi2)",
                       dest="KStest",
                       default="")
    parser.add_option ("-R","--RooStatOutput",
                       help="Histos from RooStats output",
                       dest="rooStats",
                       default="")
    parser.add_option ("--saveRatio",
                       help="Save ratios in Rootfile",
                       dest="saveRatio",
                       default="False")
    parser.add_option ("--variables",
                       help="Variables to plot",
                       dest="variables",
                       default="")
    #parser.add_option ("-s","--source",
    #                   help="Origin of the inputs: FTM, HistFitter",
    #                   dest="source",
    #                   default="FTM")
    
    # parser.add_option ("-l","--logmode",
    #                    help="log mode",
    #                    dest="logmode",
    #                    default=False)
    (options, args) = parser.parse_args()
    fdir=options.fdir
    odir=options.odir
    rebin = options.rebin
    ext = options.ext
    pref = options.pref
    uncert = options.uncert
    verbose  = options.verbose
    label = options.label
    yields = options.yields
    normsig = options.normsig
    normsig2 = options.normsig2
    normdata= options.normdata
    noratio= options.noratio
    variables= options.variables
    draw = (options.draw).split(' ')
    drawData = draw[0]
    drawSignal = draw[1]
    usePseudoData = options.usePseudoData
    useAsimovData = options.useAsimovData
    KStest = options.KStest
    rooStats = options.rooStats
    saveRatio = eval(options.saveRatio)
    #source = options.source
    # logmode = options.logmode

    print "Will create finalplots for channels "+str(channelList)+" in linear and log scales"

    for channel in channelList:
        #codir=odir+"/"+channel[:-(len(channel.split("_")[-1])+1)]
        spchan=channel.split("_")
        if len(spchan)>1: 
            if "Btag" in spchan[1]: codir=odir+"/"+spchan[0]+"/"+spchan[1][:-16]+"/"+spchan[1][6:]
            else: codir=odir+"/"+spchan[0]+"/"+spchan[1][:spchan[1].find("btag")-1]+"/"+spchan[1][6:]
        else: codir=odir# +"/"+spchan[0][:-7]+"/"+spchan[0][6:]
        # print spchan[1]
        # print spchan[1][:-7]
        # print spchan[1][6:]
        # print codir
#        if "6jetin3" in channel or  "6jetin4" in channel or  "5jetex4" in channel:
#          drawData = "False"

        st=""
        sc=""
        # if len(spchan)>1: name="\""+sc+" N_{jets}"+sj+" "+spchan[1][0]+", N_{b-tags}"+st+" "+spchan[1][6]+"\""
        # else:name="\""+sc+" N_{jets}"+sj+" "+spchan[0][0]+", N_{b-tags}"+st+" "+spchan[0][6]+"\""
        name = "Fit example"
        
        #tdir=odir.split("/")[0]
        # print fdir+rooStats+channel+".root"
        rooStatsFile = fdir+rooStats+channel+".root"
        tdir=odir
        command="python doPlotsAndYields.py --dir "+fdir+" --channel "+channel+" --name \""+name+"\"  --rebin "+str(rebin)+" --yext txt"+" --ext \""+ext+"\" --label \""+label+"\" --yields "+yields+" --draw \""+drawData+" "+drawSignal+"\""+" --normsig "+normsig+" --normdata "+normdata+" --normsig2 "+normsig2 +" --noratio "+noratio
        if variables:
          command += " --variables \""+variables+"\""
        #command +='  --alternatesignal "Stop2_500_Stop1_300_LSP_120" --alternatesignal2 "Stop2_350_Stop1_200_LSP_20" '
        #command +='  --alternatesignal "BRStop2_500_Stop1_200_LSP_20_LSPt0.0_Ht0.0_Zt1.0" --alternatesignal2 "BRStop2_500_Stop1_200_LSP_20_LSPt1.0_Ht0.0_Zt0.0" '
        #command +='  --alternatettbar "ttbarMadgraph-ttl ttbarMadgraph-ttbb ttbarMadgraph-ttcc"'
        if pref!="": command+=" -p "+pref
        if uncert!="": command+=" -u "+uncert
        if eval(usePseudoData): command+=" --usePseudoData True"
        if eval(useAsimovData): command+=" --useAsimovData True"
        if KStest!="": command+=" -k "+KStest
        if rooStats != "" : command+=" -R "+rooStatsFile
        if verbose: command+=" -v "+str(verbose)
        if options.ftm: command+=" -f "
        if saveRatio: command += " --saveRatio True"
        print command
        os.system(command + " &")
        #command+=" -l True"
        #print command
        #os.system(command)
        pass

