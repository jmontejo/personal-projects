#FIXME currently with double link naming convention, shoud remove link_old after some time
import os, sys, time
from optparse import (OptionParser,BadOptionError)

color    = '\033[91m'
endcolor = '\033[0m'

class PassThroughOptionParser(OptionParser):
    """
    An unknown option pass-through implementation of OptionParser.

    When unknown arguments are encountered, bundle with largs and try again,
    until rargs is depleted.  

    sys.exit(status) will still be called if a known argument is passed
    incorrectly (e.g. missing arguments or bad argument types, etc.)        
    """
    def _process_args(self, largs, rargs, values):
        while rargs:
            try:
                OptionParser._process_args(self,largs,rargs,values)
            except BadOptionError, e:
                pass
            except Exception, e:
                print "exception---",e

# ---- hack_file
def hack_file(file,message,silent):
  os.system("ln -s hack_file "+hack_link(file)+" 2> /dev/null ")
  bak = hack_bak(file)
  if(copy):
    if os.path.exists(bak):
      print "Backup copy already exists, please check differences and delete if needed"
      sys.exit(2)
    os.system("cp "+file+" "+bak)
  mfilename = hack_msg(file)
  if silent != 0:
    if os.path.exists(mfilename):
      #instead of sed -i, which is giving problems locally
      os.system("sed /Silent/d "+mfilename+" > "+mfilename+".dummy")
      os.system("mv -f "+mfilename+".dummy "+mfilename)
    mfile = open(mfilename,"a")
    mfile.write("Silent: "+str(time.time()+60*60*silent)+"\n")
    mfile.close()
  if message != "":
    mfile = open(mfilename,"a")
    mfile.write(time.ctime()+"\n")
    mfile.write(message+"\n")
    mfile.close()
  #else:
  #  os.system("echo "+time.ctime()+" >> "+mfilename)
  #  os.system("vim "+mfilename)

# ---- dehack_file
def dehack_file(file):
  """
    check if the -c argument was used
      print a diff and ask for confirmation
  """
  if not is_hacked(file):
    print "File was not hacked:",file
    return
  msg = hack_msg(file)
  bak = hack_bak(file)
  if os.path.exists(msg):
    print "Dehacking file:"+file
    os.system("cat "+msg)
  if os.path.exists(bak):
    print "> original"
    print "< edited"
    isdiff = os.system("diff "+file+" "+bak)
    if isdiff!=0:
      try: 
        input = ""
        while input!="y" and input!="n":
          input = raw_input("Differences with respect to the original file, restore the original? y/n, v to vimdiff, ctrl+c to cancel: ")
          if input == "v": 
            os.system("vimdiff "+bak+" "+file)
        if input == "y": os.system("cp -f "+bak+" "+file)
        os.remove(bak)
      except KeyboardInterrupt:
        print ""
        sys.exit(1)
  if os.path.exists(msg):
    os.remove(msg)
  link = hack_link(file)
  if os.path.lexists(link):
    os.remove(link)
  link_old = hack_link_old(file)
  if os.path.lexists(link_old):
    os.remove(link_old)
  if os.path.exists(bak): os.remove(bak)

# ---- check if the file is hacked
def is_hacked(file):
  link = hack_link(file)
  linkold = hack_link_old(file)
  return os.path.lexists(link) or os.path.lexists(linkold)
# ---- check if the file should be silent
def is_silent(file):
  mfilename = hack_msg(file)
  if not os.path.exists(mfilename): return False
  mfile = open(mfilename)
  for line in mfile.read().split("\n"):
    if "Silent:" in line:
      stime = float(line.split(":")[1])
      if stime > time.time():
        mfile.close()
        return True
      break
  mfile.close()
  return False

# ---- naming of hack link and message
def hack_bak(file):
  return file+".hack.bak"
def hack_link(file):
  return os.path.join(os.path.dirname(file),"hack."+os.path.basename(file)+".hack")
def hack_link_old(file):
  return file+".hack"
def hack_msg(file):
  return os.path.join(os.path.dirname(file),".message_"+os.path.basename(file)+".hack")

# ---- check vim
def check_vim(file):
  if is_hacked(file) and not is_silent(file):
    print color+file+": this file has (at least) one hack, please check"+endcolor
    msg = hack_msg(file)
    if os.path.exists(msg):
      os.system("cat "+msg)
    raw_input("")
  
# ---- check make
def check_make(makeclean):

  hacks = set()
  for item in os.listdir("."):
    if os.path.isfile(item) and is_hacked(item) \
       and not is_silent(item) and not item.endswith(".py"): hacks.add(item)
    if os.path.isdir(item):
      folderitems = os.listdir(item)
      if len(folderitems) >= 100:
        print "Skipping folder with too many files:",item,len(folderitems)
        continue
      for item2 in folderitems:
        file = os.path.join(item,item2)
        if os.path.isfile(file) and is_hacked(file) and not is_silent(file): hacks.add(file)
        
  if hacks:
    print color+"You are going to compile files with hacks, please check:"+endcolor
    for i,file in enumerate(hacks):
      print "#",i,file
      msg = hack_msg(file)
      if os.path.exists(msg):
        os.system("cat "+msg)
    if not makeclean:
      raw_input("")
  

#if __name__ == "__main__": main()
#def main():
parser = PassThroughOptionParser() 
parser.add_option ("-c","--copy", 
                   help="Make a backup copy of the original file", 
                   dest="copy", 
                   action="store_true", 
                   default=False) 
parser.add_option ("-d","--dehack", 
                   help="Dehack the given file", 
                   dest="dehack", 
                   action="store_true", 
                   default=False) 
parser.add_option ("-e","--edit", 
                   help="Make a backup copy and edit the given file", 
                   dest="edit", 
                   action="store_true", 
                   default=False) 
parser.add_option ("-s","--silent",metavar="N",type=float,
                   help="Turns off the warnings for a given file during the next N hours",
                   dest="silent", 
                   default=0) 
parser.add_option ("-m","--message", 
                   help="Add a message about the content of the hack", 
                   dest="message", 
                   default="") 
#note that running in vim or make mode has to ignore the rest of the options since 
#they could be meant for vim or make
parser.add_option ("--vim", 
                   help="Alert about a possible hack before opening the file in vim", 
                   dest="vim", 
                   action="store_true", 
                   default=False) 
parser.add_option ("--make", 
                   help="Alert about possible hacks before using the files in a make", 
                   dest="make", 
                   action="store_true", 
                   default=False) 

options,args = parser.parse_args()
# --- enable -e if there is no other option specified
hasoption = False
for value in parser.values.__dict__.itervalues():
  if value is "files": continue
  if   type(value) == type(True): hasoption = hasoption or value
  elif type(value) == type("")  : hasoption = hasoption or value != ""
  elif type(value) == float     : hasoption = hasoption or value != 0
options.edit = options.edit or not hasoption
# ---
dehack       = options.dehack
copy         = options.copy or options.edit
edit         = options.edit
message      = options.message
vim          = options.vim
make         = options.make
silent       = options.silent
makeclean    = 'clean' in args

if make:
  sys.exit( check_make(makeclean) )
if not args and not vim:
  print "hack.py needs at least one argument (when not running in --make or --vim mode)"
  sys.exit(1)

for item in args:
  if not os.path.exists(item) and not vim:
    print "Item does not exist:",item
    args.remove(item)
    continue
  if vim:
    check_vim(item)
    continue
  if dehack:
    dehack_file(item)
  else:
    hack_file(item,message,silent)
  if edit:
    os.system("\\vim "+item)
