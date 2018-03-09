#! /usr/bin/python
import sys,os, glob,shutil

def isBackground():
    return os.getpgrp() != os.tcgetpgrp(sys.stdout.fileno())

if __name__ == "__main__":

  trashbin = "$MYGIT/scripts/trash/"
  extensions = [".sh",".py",".cxx",".C",".h"]

  cmd ="/bin/rm"
  filesg = []
  for arg in sys.argv[1:]:
    cmd += " "+arg
    arg = arg.replace("(","\(").replace(")","\)")
    if "*" in arg:
      if arg == "*":
        if isBackground():
          print "Running on background, assume to NOT remove everything"
          sys.exit(2)
        input = raw_input("You are trying to remove everything, are you sure? y/[n]: ")
        if input != "y":
          sys.exit(1)
    filesg.extend(glob.glob(arg))

  for file in filesg:
    for ext in extensions:
      if file.endswith(ext):
        try: #to be able to remove files in the trashbin
          shutil.copy(file,os.path.expandvars(trashbin))
        except Exception, e:
          print e
          pass
        break

  stdout = os.popen(cmd).read()
  if stdout != "":
    print stdout
