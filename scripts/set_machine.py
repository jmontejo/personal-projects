#! /usr/bin/python
from optparse import OptionParser
import sys,os, glob,shutil

def dosetup(pc1, pc2):
  
  print "dosetup", pc1, pc2
  cmd = "ssh "+pc1+" 'if [ ! -f .ssh/id_rsa ]; then ssh-keygen -q -t rsa; else exit 1; fi'"
  print cmd
  print os.popen(cmd).read()
  cmd = "ssh "+pc2+" mkdir -p .ssh"
  print cmd
  print os.popen(cmd).read()
  cmd = "ssh "+pc1+" 'cat .ssh/id_rsa.pub' | ssh "+pc2+" 'cat >> .ssh/authorized_keys'"
  print cmd
  print os.popen(cmd).read()
  
  """
    In case it doesn't work
     * Put the public key in .ssh/authorized_keys2
     * Change the permissions of .ssh to 700
     * Change the permissions of .ssh/authorized_keys2 to 640
  """

if __name__ == "__main__":

  machinelist = os.path.expandvars("$MYGIT/scripts/machinelist.txt")
  if not os.path.exists(machinelist): os.system("touch "+machinelist)
  parser = OptionParser() 
  parser.add_option ("-l","--list", 
                     help="List all machines in the list",
                     dest="list", 
                     action="store_true", 
                     default=False) 
  parser.add_option ("-r","--remove", 
                     help="Remove machine from the list",
                     dest="remove")
  parser.add_option ("-a","--add", 
                     help="Add machine to the list. Do not set up",
                     dest="add")
  parser.add_option ("-s","--setup", 
                     help="Add machine to the list. Set up with the rest of the machines in the list",
                     dest="setup")
  parser.add_option ("-A","--all", 
                     help="Set up all possible permutations amongst machines",
                     dest="all", 
                     action="store_true", 
                     default=False) 
  parser.add_option ("-L","--local", 
                     help="Set up all machines respect to the local one",
                     dest="all", 
                     action="store_true", 
                     default=False) 
  options,args = parser.parse_args()
  if options.setup: options.add = options.setup

  if options.list:
    os.system("cat "+machinelist)
    sys.exit(0)
  if options.remove:
    if os.system("grep -q -w "+options.remove+" "+machinelist):
      print "Machine not in list"
      sys.exit(1)
    error = os.system("sed -i '/"+options.remove+"/d' "+machinelist)
    if error:
      print "Fucking sed in Mac, retrying"
      os.system("sed -ie'' '/"+options.remove+"/d' "+machinelist)
    sys.exit(0)
  if options.add:
    if not os.system("grep -q -w "+options.add+" "+machinelist):
      print "Machine already in list"
      sys.exit(1)
    os.system("echo "+options.add+" >> "+machinelist)
    sys.exit(0)

  with open(machinelist) as f:
    thelist = f.read().split("\n")

  if options.setup:
    for machine in thelist:
      if not machine: continue
      if machine == options.setup: continue
      dosetup(options.setup,machine)
      dosetup(machine,options.setup)

  if options.all:
    for i1,machine1 in enumerate(thelist):
      if not machine1: continue
      for i2,machine2 in enumerate(thelist):
        if not machine2: continue
        if i2 <= i1: continue
        if "lxplus" in machine2: continue
        dosetup(machine2, machine1)
        dosetup(machine1, machine2)

