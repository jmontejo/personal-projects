#! /usr/bin/python
from optparse import OptionParser
import sys,os

if __name__ == "__main__":

  parser = OptionParser() 
  parser.add_option ("-c","--clean", 
                     help="Remove old files after setting up",
                     dest="clean", 
                     action="store_true", 
                     default=False) 

  options,args = parser.parse_args()
  if len(args) != 1:
    print "Path to $MYSVN needed: first_setup.py $MYSVN"
    sys.exit(1)
  mysvn = args[0]+"/"

  #original:(new,command to run before, command to run after)
  files = {
    "~/.bashrc"                   :("/scripts/my_bashrc",),
    "~/.bash_profile"             :("/scripts/my_bash_profile",),
    "~/.subversion/config"        :("/scripts/my_svnconfig.edit",
                                        "sed  's/$MYSVN/%s/g' %s/scripts/my_svnconfig > %s/scripts/my_svnconfig.edit" % (mysvn.replace("/","\\/"),mysvn,mysvn) ),
    "~/.vim/colors/mymorning.vim" :("/scripts/my_morning.vim",
                                        "mkdir -p ~/.vim/colors"),
    "~/.vimrc"                    :("/scripts/my_vimrc",),
  }


  for old, new_ in files.iteritems():
    new = mysvn+new_[0]

    print "-- "+old
    if os.path.exists(os.path.expanduser(old)):
      old_bak = old+".old"
      print "Previous %s moved to %s" % (old, old_bak)
      os.system("mv %s %s" %(old,old_bak))
      if options.clean:
        os.system("rm %s" % old_bak)
    else: #remove broken symlinks
      os.system("rm -f %s" %old)

    if len(new_)>1:
      precmd = new_[1]
      os.system(precmd)
    os.system("ln -s %s %s" % (new,old))
    if len(new_)>2:
      postcmd = new_[2]
      os.system(postcmd)
