#! /usr/bin/env python2.7
import os, sys

if __name__ == "__main__":

  add_i = True
  for arg in sys.argv[1:]:
    if not arg.startswith("-"): break
    if arg == "-f":
      add_i = False
      break
  
  cmd = "/bin/cp"
  if add_i: cmd += " -i "
  for arg in sys.argv[1:]: cmd += " "+arg

  os.system(cmd)
