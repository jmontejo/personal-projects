#! /usr/bin/env python2.7

# Run with fill_metadatay.py pattern, instructions below are obsolete and include the individual steps, now wrapped
# metadata.txt obtained with:
# > lsetup  "asetup AthAnalysisBase,2.4.35" pyAMI
# > rucio ls --short mc15_13TeV.448*.MGPy8EG_A14N23LO_*N1*_higgsinoRPV*.evgen.EVNT.* | grep -v _tid | sort > evnt.txt # <--- change pattern here
# > getMetadata.py --fields="ldn,dataset_number,subprocessID,crossSection,kFactor,genFiltEff,crossSectionTotalRelUncertUP,crossSectionRef" --inDsTxt=evnt.txt --outFile="metadata.txt" --delim=";"
# see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PmgCentralPage#Reporting_issues
#
# SUSY IDs from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/SUSYSignalUncertainties#Subprocess_IDs
import os, sys, argparse, shutil

N1N2xsecmap = {}
C1N1xsecmap = {}
Gluinoxsecmap = {}
Stopxsecmap = {}
SlepRxsecmap = {}
def fillXsec():
    folder = os.path.expandvars("$MYGIT/scripts/")
    with open(folder+"/officialSlepR_xsec.txt") as infile:
        for line in infile:
            if line.startswith("#"): continue
            mass, xsec, uncDown, uncUp = line.split()
            unc = (-float(uncDown)+float(uncUp))/2
            SlepRxsecmap[mass] = (str(float(xsec)*1e-3),str(float(unc)/100.)) #xsec from pb to nb, uncertainty in %
    with open(folder+"/officialN1N2_xsec.txt") as infile:
        for line in infile:
            if line.startswith("#"): continue
            mass, xsec, unc = line.split()
            N1N2xsecmap[mass] = (str(float(xsec)*1e-6),str(float(unc)/float(xsec))) #xsec from fb to nb, relative uncertainty
    with open(folder+"/officialC1N1_xsec.txt") as infile:
        for line in infile:
            if line.startswith("#"): continue
            mass, xsec, unc = line.split()
            C1N1xsecmap[mass] = (str(float(xsec)*1e-6),str(float(unc)/float(xsec))) #xsec from fb to nb, relative uncertainty
    with open(folder+"/officialGluino_xsec.txt") as infile:
        for line in infile:
            if line.startswith("#"): continue
            mass, xsec, unc = line.split()
            Gluinoxsecmap[mass] = (str(float(xsec)*1e-3),str(float(unc)/100.)) #xsec from pb to nb, uncertainty in %
    with open(folder+"/officialTT_xsec.txt") as infile:
        for line in infile:
            if line.startswith("#"): continue
            mass, xsec, unc = line.split()
            Stopxsecmap[mass] = (str(float(xsec)*1e-3),str(float(unc)/100.)) #xsec from pb to nb, uncertainty in %

def runCommands(pattern, exclude):
    command = '''
    (type ami &> /dev/null && type rucio &> /dev/null ) || source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh; lsetup "asetup AthAnalysisBase,2.4.35" pyAMI; lsetup rucio
    rucio ls --short {pattern} | grep -v _tid | sort | cut -f2 -d":" | grep -v {exclude} > evnt.txt
    getMetadata.py --fields="ldn,dataset_number,subprocessID,crossSection,kFactor,genFiltEff,crossSectionTotalRelUncertUP,crossSectionRef" --inDsTxt=evnt.txt --outFile="metadata.txt" --delim=";"
    '''.format(pattern=pattern, exclude=exclude)
    os.system(command)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("pattern",help="Pattern to match datasets. E.g. 'mc15_13TeV.4487*.MGPy8EG_A14N23LO_*GG*_higgsinoRPV*.evgen.EVNT.*'")
    parser.add_argument("--exclude", "-e",help="Pattern to exclude from the matched datasets.",default="WTFmatchesThis")
    opts = parser.parse_args()
    if not opts.pattern.startswith("mc15_13TeV"):
        print "pattern must start with mc15_13TeV"
        sys.exit(1)
    if not "evgen.EVNT" in opts.pattern:
        print "pattern must contain evgen.EVNT"
        sys.exit(2)

    fillXsec()
    runCommands(opts.pattern, opts.exclude)

    with open("metadata.txt") as infile:
      with open("metadata.txt.out","w") as outfile:
        for line in infile:
          tokens = line.split(";")
          if "EVNT" in line and not line.startswith("#"):
            mass = line.split(".")[2].split("_")[4]
            print tokens[0], "mass:",mass
            if "C1N1" in line:
                tokens[2] = "0" #"115" PMG complains, there has to a zero, and duplicated lines with other ids if desired
                tokens[3] = C1N1xsecmap[mass][0]
                tokens[6] = C1N1xsecmap[mass][1]
            elif "N1N2" in line:
                tokens[2] = "0" #"112" PMG complains, there has to a zero, and duplicated lines with other ids if desired
                tokens[3] = N1N2xsecmap[mass][0]
                tokens[6] = N1N2xsecmap[mass][1]
            elif "GG" in line:
                tokens[2] = "0" #actually 2 but nobody uses this
                tokens[3] = Gluinoxsecmap[mass][0]
                tokens[6] = Gluinoxsecmap[mass][1]
            elif "TT" in line:
                tokens[2] = "0"
                tokens[3] = Stopxsecmap[mass][0]
                tokens[6] = Stopxsecmap[mass][1]
            elif "Smuon" in line:
                tokens[2] = "0"
                tokens[3] = SlepRxsecmap[mass][0]
                tokens[6] = SlepRxsecmap[mass][1]
            else:
                print "PRODUCTION MECHANISM NOT FOUND, NO XSEC:",line
            tokens[7] = "XsecSUSY"
            outfile.write(";".join(tokens)+"\n")
          else:
            outfile.write(line)
    shutil.move("metadata.txt", "metadata.txt.old")
    shutil.move("metadata.txt.out", "metadata.txt")
    print "Original file in metadata.txt.old, updated file in metadata.txt"

if __name__ == "__main__": main()
