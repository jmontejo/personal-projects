from optparse import OptionParser
import sys,os,subprocess
import ROOT as root
from random import randint

def compareHisto(histo1, histo2, file1, file2):

  if histo1.GetNbinsX() != histo2.GetNbinsX():
    print "Different number of bins:",
    print file1,":",histo1.GetNbinsX()
    print file2,":",histo2.GetNbinsX()
    return False
  else:
    #print "Same number of bins"
    for i in xrange(histo1.GetNbinsX()+2):
        if histo1.GetBinContent(i) != histo2.GetBinContent(i):
            print "Different content:",i,histo1.GetBinContent(i), histo2.GetBinContent(i)
            break
            return False
    else:
            #print "Same content"
            return True


if __name__ == "__main__":

  parser = OptionParser() 
  parser.add_option("-n", "--name", 
                    dest="histoname", 
                    help="Specify histo name",
                    default="")
  parser.add_option ("-v","--verbose", 
                     help="Turn on verbose printout. Default is False", 
                     dest="verbose", 
                     action="store_true", 
                     default=False) 
  
  options,args = parser.parse_args() 

  if len(args) != 2:
    print "Need to specify the two files to compare"
    sys.exit(1)
  
  file1 = os.path.abspath(args[0])
  file2 = os.path.abspath(args[1])

  if not os.path.exists(file1):
    print "File doesn't exist: ",file1
    sys.exit(1)
  if not os.path.exists(file2):
    print "File doesn't exist: ",file2
    sys.exit(1)

  rfile1 = root.TFile.Open(file1)
  rfile2 = root.TFile.Open(file2)
  if not rfile1 or not rfile1.IsOpen():
    print "File couldn't be opened: ",file1
    sys.exit(2)
  if not rfile2 or not rfile2.IsOpen():
    print "File couldn't be opened: ",file2
    sys.exit(2)
  diff = os.system("diff %s %s &> /dev/null " %(file1,file2) )
  if diff==0:
    print "Exactly the same files:",file1,file2
    sys.exit(0)
    
  dir = rfile1
  path = ""
  if not options.histoname:
    for key in [x.GetName() for x in dir.GetListOfKeys()]:
      obj = rfile1.Get(path+key)
      if isinstance(obj, root.TH1):
        options.histoname = key
        #print "Using first TTree with name:",path+key
        histo1 = rfile1.Get(path+options.histoname)
        histo2 = rfile2.Get(path+options.histoname)
        if not histo2:
          print "Couldn't find TTree", path+options.histoname, "in file",file2
          sys.exit(4)
        equal = compareHisto(histo1,histo2, file1, file2)
        if not equal: 
            print path+key
            break
      elif isinstance(obj, root.TDirectoryFile):
        dir = obj
        path += key+"/"
  else:
        histo1 = rfile1.Get(path+options.histoname)
        histo2 = rfile2.Get(path+options.histoname)
        if not histoname2:
          print "Couldn't find TTree", path+options.histoname, "in file",file2
          sys.exit(4)
        compareHisto(histo1,histo2, file1, file2)
