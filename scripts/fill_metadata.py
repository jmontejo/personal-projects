# metadata.txt obtained with:
# > lsetup  "asetup AthAnalysisBase,2.4.35" pyAMI
# > getMetadata.py --fields="ldn,dataset_number,subprocessID,crossSection,kFactor,genFiltEff,crossSectionTotalRelUncertUP,crossSectionRef" --inDsTxt=evnt.txt --outFile="metadata.txt" --delim=";"
# see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/PmgCentralPage#Reporting_issues
#
# SUSY IDs from https://twiki.cern.ch/twiki/bin/view/AtlasProtected/SUSYSignalUncertainties#Subprocess_IDs

N1N2xsecmap = {}
with open("officialN1N2_xsec.txt") as infile:
    for line in infile:
        if line.startswith("#"): continue
        mass, xsec, unc = line.split()
        N1N2xsecmap[mass] = (str(float(xsec)*1e-6),str(float(unc)/float(xsec))) #xsec from fb to nb, relative uncertainty
C1N1xsecmap = {}
with open("officialC1N1_xsec.txt") as infile:
    for line in infile:
        if line.startswith("#"): continue
        mass, xsec, unc = line.split()
        C1N1xsecmap[mass] = (str(float(xsec)*1e-6),str(float(unc)/float(xsec))) #xsec from fb to nb, relative uncertainty

with open("metadata.txt") as infile:
  with open("metadata.txt.out","w") as outfile:
    for line in infile:
      tokens = line.split(";")
      if "EVNT" in line and not line.startswith("#"):
        mass = line.split("_")[5][:3]
        print tokens
        if "C1N1" in line:
            tokens[2] = "115"
            tokens[3] = C1N1xsecmap[mass][0]
            tokens[6] = C1N1xsecmap[mass][1]
        if "N1N2" in line:
            tokens[2] = "112"
            tokens[3] = N1N2xsecmap[mass][0]
            tokens[6] = N1N2xsecmap[mass][1]
        tokens[7] = "XsecSUSY"
        outfile.write(";".join(tokens)+"\n")
      else:
        outfile.write(line)
