import glob
import shutil

for f in glob.glob("mc16_13TeV.501*/*root.1"):
    dsid = int(f.split(".")[1])
    if "r9364" in f: campaign = "mc16a"
    if "r10201" in f: campaign = "mc16d"
    if "r10724" in f: campaign = "mc16e"
    outf = "DSID501xxx/pileup_{campaign}_dsid{dsid}_AFII.root".format(campaign=campaign,dsid=dsid)
    shutil.copy(f,outf)
