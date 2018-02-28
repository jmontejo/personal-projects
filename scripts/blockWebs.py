#! /usr/bin/python
from optparse import OptionParser
import sys,os, glob,shutil, time

if __name__ == "__main__":

  parser = OptionParser() 
  parser.add_option ("-a","--allow", 
                     help="Allow sites", 
                     dest="allow", 
                     action="store_true", 
                     default=False) 
  parser.add_option ("-b","--block", 
                     help="Block sites", 
                     dest="block", 
                     action="store_true", 
                     default=False) 

  
  options,args = parser.parse_args() 
  block = options.block
  allow = options.allow

  if block and allow:
    print "Block *and* Allow is not valid" 
    sys.exit(1)
  if not block and not allow:
    print "Block or Allow is mandatory" 
    sys.exit(1)
  if allow:
    print "FUCKER!!! DON'T GOOF OFF, USE YOUR TIME WISELY!!!" 
    time.sleep(10)
    sys.exit(1)

  #file="hosts"
  #tempfile="hosts.tmp"
  file="/etc/hosts"
  tempfile="/etc/hosts.tmp"
  edit=""
  for line in open(file).read().split("\n"):
    if "www" in line or ".com" in line:
      if block and line.startswith("#"):
        line = line[1:]
        print "Blocking:",line
      if allow and not line.startswith("#"):
        print "Allowing:",line
        line = "#"+line
    if line != "":
      edit += line+"\n"
  open(tempfile,"w").write(edit)
  shutil.copy(tempfile,file)

  os.popen("dscacheutil -flushcache")
