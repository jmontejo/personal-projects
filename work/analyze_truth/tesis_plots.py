import os

jos = [
"jo_extHFtype.py","jo_extHFtype_cc.py",
"jo_kin.py",
"jo_kin_cc.py"
]
samples = {
           #"default":"PowhegPythiaMPIp15onlyHF MadgraphMPI_nominalp15onlyHF Sherpa2_defaultp15",
           #"defaultmgpp":"PowhegPythiaMPIp15onlyHF MadgraphMPI_nominalp15onlyHF",
           #"defaultcc":"PowhegPythiaMPIp15onlyHF MadgraphMPI_nominalp15onlyHF",
           "ph":"PowhegPythiaMPIp15onlyHF MadgraphMPI_nominalp15onlyHF Sherpa2_defaultp15 PowhegHerwigMPIp15onlyHF",
           "phcc":"PowhegPythiaMPIp15onlyHF MadgraphMPI_nominalp15onlyHF PowhegHerwigMPIp15onlyHF",
           #"scales":"Sherpa2_defaultp15 Sherpa2_defaultX05p15 Sherpa2_defaultX2p15 Sherpa2_R_Mbbp15 Sherpa2_Q_CMMPSp15 -c 2",
           #"other":"Sherpa2_defaultp15 Sherpa2_NNPDFp15 Sherpa2_MSTWp15 Sherpa2_CSS_KINp15 -c 2",
           #"mgcc":" ".join([x+"p25onlyHFnoMPI" for x in "MadgraphOff_nominal MadgraphOff_Q2down MadgraphOff_Q2up MadgraphOff_Matchup MadgraphOff_Mcup MadgraphOff_Mcdown".split()])+" -c 2 -z",
           #"meps":"MadgraphOff_nominalwithMEb MadgraphOff_nominalwithMEb_ME0b MadgraphOff_nominalwithMEb_ME1b MadgraphOff_nominalwithMEb_ME2b -c 2 -m",
           #"mepspp":"PowhegPythiaMPIp15withMEb PowhegPythiaMPIp15withMEb_ME0b PowhegPythiaMPIp15withMEb_ME1b -c 2 -m",
        }

for jo in jos:
  for name,sample in samples.iteritems():
    if "cc" in jo and not "cc" in name: continue
    if "cc" not in jo and "cc" in name: continue
    cmd = "python plotSamples.py %s -j %s -p %s" %(sample, jo, name)
    if "extHFtype" in jo or "meps" in name:
      print cmd
      os.system(cmd)
    cmd += " -n"
    if not "meps" in name:
      print cmd
      os.system(cmd)

#os.system("python plotSamples.py PowhegPythiaMPIref PowhegPythiaMPIhigh  -j jo_extHFtype.py -p mpi")
#os.system("python plotSamples.py Sherpa2_defaultp15 PowhegPythiaMPIp15onlyHF PowhegPythiaMPIp15onlyHF_norw -j jo_kin_tt.py -p norw -c 1 -n")
