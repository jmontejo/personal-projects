#!/usr/bin/env python2
from optparse import OptionParser
import sys,os,subprocess
import ROOT as root
from random import randint
import re

def compareTree(tree1, tree2, file1, file2, options):

  sameEntries = False
  if tree1.GetEntries() != tree2.GetEntries():
    print "Different number of entries:",
    print file1,":",tree1.GetEntries()
    print file2,":",tree2.GetEntries()
    if tree2.GetEntries() < tree1.GetEntries():
      tmp   = tree2
      tree2 = tree1
      tree1 = tmp
  else:
    sameEntries = True
    print "Same number of entries"

  branches1 = set([x.GetName() for x in tree1.GetListOfBranches() if not options.skip or not re.search(options.skip,x.GetName())])
  branches2 = set([x.GetName() for x in tree2.GetListOfBranches() if not options.skip or not re.search(options.skip,x.GetName())])

  if branches1 != branches2:
    print "Difference in branches:"
    print "In 1 but not in 2: ",branches1.difference(branches2)
    print "In 2 but not in 1: ",branches2.difference(branches1)
    common = branches1.intersection(branches2)
  else:
    print "Same set of branches"
    common = branches1

  branchesmap = dict([(x.GetName(),x.GetTitle()) for x in tree1.GetListOfBranches() if x.GetName() in common])
  for key, val in branchesmap.iteritems():
    if not tree1.GetLeaf(val):
      if tree1.GetLeaf(key): branchesmap[key] = key
      elif tree1.GetLeaf(val[:val.find("/")]): branchesmap[key] = val[:val.find("/")]
      else:
        print "Crap, no way of getting leaf?",key,val

  if options.novectors:
    common  = [c for c in common if "vector" not in  tree1.GetLeaf(c).GetTypeName()]
  ##common = [c for c in common if "vector" not in  tree1.GetLeaf(branchesmap[c]).GetTypeName()]
  #print common

  index = None
  if   "EventNumber" in branches2: index = "EventNumber"
  elif "eventNumber" in branches2: index = "eventNumber"
  else:
    for b in branches2:
      if re.search(".vent.*umber", b):
        index = b
        break
    else:
      print "Could not build index, no event number in the file"
      if not sameEntries: return
      print "Will assume that the events are in the same order in both trees"

  if index: tree2.BuildIndex(index,"mc_channel_number")
  crazyset = set()
  limit = min(tree1.GetEntries(),tree2.GetEntries())
  options.nevents = min(options.nevents,limit)
  print "Started scan"
  for count in range(options.nevents):
    i = randint(0,limit)
    if index:
      header = "-------- %s: %s"%(index,getattr(tree1,index))
      while True: #Scan events until it finds a coincidence
        tree1.GetEntry(i)
        ret = tree2.GetEntryWithIndex(getattr(tree1,index),getattr(tree1,"mc_channel_number"))
        if ret!=-1: break
        else: i = randint(0,limit)
    else: 
      header = "-------- Event count: %d"%count
      tree1.GetEntry(i)
      tree2.GetEntry(i)
    maxlength = len(max(common, key=len))
    firstVar = True
    for v in sorted(common):
      v1 = getattr(tree1,v)
      vtype = type(v1)
      if vtype is int or vtype is long or vtype is float:
        v2 = getattr(tree2,v)
        if abs(v1 - v2) > options.threshold:
          if (v1 < 1e-10 or v2 < 1e-10 or v1 > 1e10 or v2 > 1e10) and not options.showcrazy: 
            crazyset.add(v)
            continue
          if firstVar: print header
          print "{0:{1}}:{2:15n} {3:15n}".format(v,maxlength+1,v1,v2)
          firstVar = False
      else:
        v2 = getattr(tree2,v)
        if v1!=v2:
          if firstVar: print header
          print v, [x for x in v1], [x for x in v2]
          #print "{0:{1}}:{2:15n} {3:15n}".format(v,maxlength+1,v1,v2)
          firstVar = False
  print "Scanned %d events and %d variables"% (options.nevents, len(common))
  if crazyset:
    print "Hidden (crazy) differences: ",crazyset

if __name__ == "__main__":

  parser = OptionParser() 
  parser.add_option("-t", "--tree", 
                    dest="tree", 
                    help="Specify tree name",
                    default="")
  parser.add_option("-T", "--tree2", 
                    dest="tree2", 
                    help="Specify second tree name if different from the first",
                    default="")
  parser.add_option("-S", "--skip", 
                    dest="skip", 
                    help="Skip branches matching this patter. Wildcards are allowed.",
                    default="")
  parser.add_option("--threshold", 
                    help="Numerical threshold to consider values as different. Default 1e-9",
                    default=1e-9)
  parser.add_option("-n", "--nevents", type="int",
                    help="Number of events to scan for differences, default 10.",
                    default=10)
  parser.add_option ("-v","--verbose", 
                     help="Turn on verbose printout. Default is False", 
                     dest="verbose", 
                     action="store_true", 
                     default=False) 
  parser.add_option ("-V","--no-vectors", 
                     help="Don't print differences in vector variables", 
                     dest="novectors", 
                     action="store_true", 
                     default=False) 
  parser.add_option ("-s","--show-crazy", 
                     help="Show also branches in the comparison with crazy values. Threshold is e10 or e-10. Default is False", 
                     dest="showcrazy", 
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
  while not options.tree:
    for key in [x.GetName() for x in dir.GetListOfKeys()]:
      obj = rfile1.Get(path+key)
      if isinstance(obj, root.TTree):
        options.tree = key
        print "Using first TTree with name:",path+key
        break
      if isinstance(obj, root.TDirectoryFile):
        dir = obj
        path += key+"/"
        break
  if not options.tree:
    print "Couldn't find a TTree in the file, please check"
    print [x.GetName() for x in rfile1.GetListOfKeys()]
    sys.exit(3)
  if not options.tree2:
    options.tree2 = options.tree

  tree1 = rfile1.Get(path+options.tree)
  tree2 = rfile2.Get(path+options.tree2)
  if not tree2:
    print "Couldn't find TTree", path+options.tree, "in file",file2
    sys.exit(4)


  compareTree(tree1,tree2, file1, file2, options)
