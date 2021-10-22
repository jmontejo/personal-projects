#! /usr/bin/env python2.7
import os, sys

def mPrint(args):
  print args

def dccp(cmdline):
  """
    dccp is defined as a function so that recursive calls can be used.
    Calling again dccp in os.system() uses a new shell where my_dccp.py is not overwriting the built in dccp.
  """
  
  toCopy = []
  cmd = "dccp "
  for arg in cmdline[1:]: #if no argument is given consider it the current folder
    if not arg.startswith("-"): toCopy.append(arg)
    else: cmd = cmd + arg + " "

  if len(toCopy) < 2:
    print "Usage: dccp src [...] dst"
    sys.exit(1)
  if len(toCopy) > 2 and not os.path.isdir(toCopy[-1]):
    print "dccp: Destination is not a folder"
    sys.exit(2)


  # check for dir -> dir TODO
  for item in toCopy[:-1]:
    if item.endswith("/"): item = item[:-1]
    if os.path.isdir(item):
      if os.path.exists(toCopy[-1]):
        outdir = os.path.join(toCopy[-1],os.path.basename(item))
      else:
        outdir = os.path.join(os.path.dirname(item),toCopy[-1])
      os.system("mkdir "+outdir)
      for subitem in os.listdir(item):
        dccp((cmd+os.path.join(item,subitem)+" "+outdir).split())
    else:
      if os.path.exists(toCopy[-1]):
        print item+" -> "+toCopy[-1]
        os.system(cmd+item+" "+toCopy[-1])

if __name__ == "__main__":
  dccp(sys.argv)

######### v1
#if __name__ == "__main__":
#
#  toCopy = []
#  cmd = "dccp "
#  for arg in cmdline[1:]: #if no argument is given consider it the current folder
#    if not arg.startswith("-"): toCopy.append(arg)
#    else: cmd = cmd + arg + " "
#
#  if len(toCopy) < 2:
#    print "Usage: dccp src [...] dst"
#    sys.exit(1)
#  if len(toCopy) == 2:
#    print cmd+toCopy[0]+" "+toCopy[1]
#    #os.system(cmd+toCopy[0]+" "+toCopy[1])
#  else:
#    if not os.path.isdir(toCopy[-1]):
#      print "dccp: Destination is not a folder"
#      sys.exit(2)
#    else:
#      for item in toCopy[:-1]:
#        print cmd+item+" "+toCopy[-1]
#        #os.system(cmd+item+" "+toCopy[-1])
#  
