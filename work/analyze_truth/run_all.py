import os, sys

for sample in sys.argv[1:]:
  #if os.path.exists("analyze_truth_outfiles/"+sample+".root"): continue

  print "Run",sample
  scriptname = "run_"+sample+".sh"
  cmd  = "cd /nfs/at3users/users/jmontejo/projects/work/scripts_work/alpgen_particle_jets/analyze_truth\n"
  cmd += "source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh --quiet \n"
  cmd += "localSetupROOT  5.34.17-x86_64-slc6-gcc4.7  --skipConfirm --quiet\n"
  cmd += "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/nfs/pic.es/user/j/jmontejo/scratch/FolderProduction_V23_stolenAndrea/edit_mc12b/RootCore/lib\n"
  cmd += "./analyze_truth "+sample+" -s withMEb -o \n"
  script = open(scriptname,"w")
  script.write(cmd)
  script.close()
  os.system("chmod a+x "+scriptname)
  #os.system(scriptname)
  os.system("qsub -q at3 "+scriptname)
