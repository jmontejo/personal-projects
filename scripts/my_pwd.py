#! /usr/bin/python
import os, sys

if __name__ == "__main__":

  real = False
  files = []
  for arg in sys.argv[1:]: #if no argument is given consider it the current folder
    if not arg.startswith("-"): break
  else:
    sys.argv.append(".")
    
  for arg in sys.argv[1:]:
    if not arg.startswith("-"):
      if not os.path.exists(arg):
        print "File/folder does not exist:",arg
      else:
        files.append(arg)
    elif arg == "-P":
      real = True
    else:
      print "Skipping unrecognized option:",arg

  for file in files:
    if real:
      print os.path.realpath(file)
    else:
      print os.path.abspath(file)
