#! /usr/bin/env python2.7
from optparse import OptionParser
import sys,os,subprocess

class pathDiff:
  
  nomatch = "#"
  def __init__(self,path1,path2,extension,exceptExtension,key,keyveto,folderveto,all,no_binary_ext,maxlevel,level=0):
    if maxlevel and level > maxlevel:
      self.skip = True
      return
    if verbose:
      print "Comparing",path1,"and",path2  
    self.name1 = os.path.basename(path1)
    self.name2 = os.path.basename(path2)
    self.original1 = path1
    self.original2 = path2
    self.subPaths = []
    self.final = False
    self.diff = False
    self.skip = False
    self.level = level
    fmt = "%"+str(level*2+2)+"s"
    self.head = fmt % "- "
    self.length = len(self.head + self.name1)+1
    if not all and (self.name1.startswith(".") or self.name2.startswith(".")):
      self.skip = True
      return
    if os.path.isdir(path1) and os.path.isdir(path2):
      if folderveto and (folderveto in path1 or folderveto in path2):
        self.skip = True
        return
      sub1 = os.listdir(path1)
      sub2 = os.listdir(path2)
      pairs = []
      for i in sub1:
        if i in sub2:
          pairs.append([os.path.join(path1,i),os.path.join(path2,i)])
        else:
          pairs.append([os.path.join(path1,i),self.nomatch])
      for i in sub2:
        if i not in sub1:
          pairs.append([self.nomatch,os.path.join(path2,i)])

      for pair in pairs:
        subDiff = pathDiff(pair[0],pair[1],extension,exceptExtension,key,keyveto,folderveto,all,no_binary_ext,maxlevel,level+1)
        if not subDiff.skip:
          self.diff = self.diff or subDiff.isDiff()
          self.length = max(self.length,subDiff.length)
          self.subPaths.append(subDiff)

    elif os.path.isfile(path1) and os.path.isfile(path2):
      if missingonly:
        self.skip = True
        return
      self.final = True
      if not self.name1.endswith(extension) or not self.name2.endswith(extension):
       self.skip = True
       return
      if not key in self.name1 or not key in self.name2:
       self.skip = True
       return
      if keyveto and (keyveto in self.name1 or keyveto in self.name2):
       self.skip = True
       return
      if no_binary_ext:
        if not "." in self.name1 or not "." in self.name2:
          self.skip = True
          return
      for ex in exceptExtension:
        if self.name1.endswith(ex) or self.name2.endswith(ex):
          self.skip = True
          return
      command = "diff -q -w "+path1+" "+path2+" &> /dev/null"
      try:
        retcode = subprocess.call(command, shell=True)
        if retcode < 0:
          print >>sys.stderr, "Child was terminated by signal", -retcode
          sys.exit(1)
        elif retcode == 0:
          self.diff = False
        else:
          self.diff = True
          #print >>sys.stderr, "Child returned", retcode
      except OSError, e:
        print >>sys.stderr, "Execution failed:", e
        sys.exit(1)
    else:
      if not missing:
        self.skip = True
        return
      else:
        self.diff = True
      for ex in exceptExtension:
        if self.name1.endswith(ex) or self.name2.endswith(ex):
          self.skip = True
          return
      if not self.name1.endswith(extension) and not self.name2.endswith(extension):
        self.skip = True
        return
    
  def isDiff(self):
    return self.diff

  def VimDiff(self):
    if self.skip: return
    if self.diff and self.final:
      command = "vimdiff "+self.original1+" "+self.original2
      try:
        retcode = subprocess.call(command, shell=True)
        if retcode < 0:
          print >>sys.stderr, "Child was terminated by signal", -retcode
          sys.exit(1)
      except OSError, e:
        print >>sys.stderr, "Execution failed:", e
        sys.exit(1)
    for i in self.subPaths:
      i.VimDiff()

  def Print(self,maxprintlevel,length=0):
    if self.skip: return
    if skipmatch and not self.diff: return
    if maxprintlevel and self.level > maxprintlevel: return

    if length == 0: length = min(self.length+1,columns)
    cut1 = columns -5
    cut2 = columns -10
    fmt="%-"+str(length)+"s%-5s%s"
    isDiff = ""
    if self.diff:
      if self.final:
        isDiff = " X "
      else:
        isDiff = "(x)"
    else:
      isDiff = " | "
    if os.path.isdir(self.original1):
      self.name1 += "/"
    if os.path.isdir(self.original2):
      self.name2 += "/"
    if len(self.head+self.name1)>cut1 or len(self.head+self.name2)>cut1:
      t1 = self.name1
      t2 = self.name2
      head = self.head
      while len(t1)>0 or len(t2)>0:
        print fmt % (head+t1[:cut2],isDiff,head+t2[:cut2])
        t1=t1[cut2:]
        t2=t2[cut2:]
        fmt2 = "%"+str(len(head))+"s"
        head = fmt2 % " "
        
    else:
      print fmt % (self.head+self.name1,isDiff,self.head+self.name2)
    for i in self.subPaths:
      i.Print(maxprintlevel,length)

if __name__ == "__main__":

  parser = OptionParser() 
  parser.add_option("-f", "--full", 
                    dest="full", 
                    help="Make a full comparison, vimdiff the content of the files", 
                    action="store_true", 
                    default=False)
  parser.add_option("-e", "--extension", 
                    dest="extension", 
                    help="Compare only files with the given extension", 
                    default="")
  parser.add_option("-c", "--columns", 
                    dest="columns", 
                    help="Specify the width of the two columns when printing", 
                    default=-1)
  parser.add_option("-a", "--all", 
                    dest="all", 
                    help="Iterate over all files and folders, including hidden ones", 
                    action="store_true", 
                    default=False) 
  parser.add_option("-m", "--missing", 
                    dest="missing", 
                    help="Show files with no match", 
                    action="store_true", 
                    default=False) 
  parser.add_option("-M", "--missing-only", 
                    dest="missingonly", 
                    help="Show only files with no match", 
                    action="store_true", 
                    default=False) 
  parser.add_option("-s", "--skip", 
                    dest="skipmatch", 
                    help="Skip matching files", 
                    action="store_true", 
                    default=False) 
  parser.add_option("-b", "--no-binary", 
                    dest="no_binary", 
                    help="Skip binary files (non-text)", 
                    action="store_true", 
                    default=False) 
  parser.add_option("-B", "--no-binary-noext", 
                    dest="no_binary_ext", 
                    help="Skip binary files (non-text). Files without extension will be considered as binaries", 
                    action="store_true", 
                    default=False) 
  parser.add_option("-E", "--except", 
                    dest="excepted", 
                    help="Compare everything but files with the given extension", 
                    default="whatfilewouldhavethisending?")
  parser.add_option("-k", "--key", 
                    dest="key", 
                    help="Consider only files that match this key", 
                    default="")
  parser.add_option("-K", "--keyveto", 
                    dest="keyveto", 
                    help="Skip files that match this key", 
                    default="")
  parser.add_option("-F", "--folderveto", 
                    dest="folderveto", 
                    help="Skip folders that match this key", 
                    default="")
  parser.add_option ("-v","--verbose", 
                     help="Turn on verbose printout. Default is False", 
                     dest="verbose", 
                     action="store_true", 
                     default=False) 
  parser.add_option("-l", "--max-level", 
                    dest="maxlevel", 
                    help="Maximum depth to check in the folder hierarchy", 
                    default=0)
  parser.add_option("-L", "--max-print-level", 
                    dest="maxprintlevel", 
                    help="Maximum depth to print. The folder hierarchy is further checked and internal differences are shown in the folder status", 
                    default=0)
  
  options,args = parser.parse_args() 

  # if no column option has been specified try to read columns as half width of the terminal
  try:
    columns      = int(options.columns)
    if columns <= 10 and columns > 0:
      print "+++Warning+++ The width has to be at least 15 for a proper display"
      print "+++Warning+++ Setting to 15"
      columns = 15
  except ValueError: 
    print "+++Error+++ Incorrect value for columns:",options.columns
    print "+++Error+++ Will read the width from the terminal size"
    columns = -1
  if columns < 0:
    sh_rows, sh_columns = os.popen('stty size', 'r').read().split()
    sh_columns = int(sh_columns)
    if sh_columns > 0: columns = (sh_columns-1)/2
    else: columns = 60
  #--
  try:
    maxlevel      = int(options.maxlevel)
    if maxlevel < 0:
      print "+++Warning+++ Maximum level has to be possitive, or zero for no limit"
      print "+++Warning+++ Setting to 'no limit'"
      maxlevel = 0
  except ValueError: 
    print "+++Error+++ Incorrect value for maxlevel:",options.maxlevel
    print "+++Error+++ Will set it to 'no limit'"
    maxlevel = 0
  #--
  try:
    maxprintlevel      = int(options.maxprintlevel)
    if maxprintlevel < 0:
      print "+++Warning+++ Maximum print level has to be possitive, or zero for no limit"
      print "+++Warning+++ Setting to 'no limit'"
      maxprintlevel = 0
  except ValueError: 
    print "+++Error+++ Incorrect value for maxprintlevel:",options.maxprintlevel
    print "+++Error+++ Will set it to 'no limit'"
    maxprintlevel = 0

  full         = options.full
  all          = options.all
  skipmatch    = options.skipmatch
  missing      = options.missing  or options.missingonly
  missingonly  = options.missingonly
  extension    = options.extension
  excepted     = options.excepted
  key          = options.key     
  keyveto      = options.keyveto     
  folderveto   = options.folderveto     
  verbose      = options.verbose 
  no_binary_ext= options.no_binary_ext
  no_binary    = options.no_binary or options.no_binary_ext

  if verbose:
    print "Options:"
    print "  verbose       :",verbose
    print "  full          :",full
    print "  all           :",all
    print "  skip          :",skipmatch
    print "  missing       :",missing
    print "  missingonly   :",missingonly
    print "  extension     :",extension
    print "  excepted      :",excepted
    print "  key           :",key
    print "  keyveto       :",keyveto
    print "  folderveto    :",folderveto
    print "  columns       :",columns
    print "  no binary     :",no_binary
    print "  no binary_ext :",no_binary_ext
  
  if len(args) != 2:
    print "Need to specify the two paths to compare"
    sys.exit(1)
  
  path1 = os.path.abspath(args[0])
  path2 = os.path.abspath(args[1])
  if path1[-1:] == "/":
    path1 = path1[:-1]
  if path2[-1:] == "/":
    path2 = path2[:-1]

  exceptExtension = [excepted]
  if no_binary:
    exceptBinaries = [".root",".so",".o",".d",".a",".0",".la",".pyc"]
    exceptExtension.extend(exceptBinaries)

  theDiff = pathDiff(path1,path2,extension,exceptExtension,key,keyveto,folderveto,all,no_binary_ext,maxlevel)
  theDiff.Print(maxprintlevel)
  if full:
    theDiff.VimDiff()

