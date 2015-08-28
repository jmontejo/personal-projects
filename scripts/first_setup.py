#! /usr/bin/python
from optparse import OptionParser
import sys,os

if __name__ == "__main__":

  parser = OptionParser() 
  #parser.add_option ("-L","--local", 
  #                   help="Set up all machines respect to the local one",
  #                   dest="all", 
  #                   action="store_true", 
  #                   default=False) 
  options,args = parser.parse_args()
  if len(args) != 1:
    print "Path to $MYSVN needed: first_setup.py $MYSVN"
    sys.exit(1)
  mysvn = args[0]+"/"

  print "-- Link .bashrc .bash_profile"
  if os.path.exists(os.path.expanduser("~/.bashrc")):
    os.system("mv ~/.bashrc ~/.bashrc.old")
    print "Previous .bashrc moved to .bashrc.old"
  os.system("ln -s "+mysvn+"/scripts/my_bashrc ~/.bashrc")
  if os.path.exists(os.path.expanduser("~/.bash_profile")):
    os.system("mv ~/.bash_profile ~/.bash_profile.old")
    print "Previous .bash_profile moved to .bash_profile.old"
  os.system("ln -s "+mysvn+"/scripts/my_bash_profile ~/.bash_profile")

  print "-- Link .subversion/config"
  if os.path.exists(os.path.expanduser("~/.subversion/config")):
    os.system("mv ~/.subversion/config ~/.subversion/config.old")
    print "Previous .subversion/config moved to .subversion/config.old"
  os.system("ln -s "+mysvn+"/scripts/my_svnconfig ~/.subversion/config")

  print "-- Link .vim/colors/mymorning.vim"
  if os.path.exists(os.path.expanduser("~/.vim/colors/mymorning.vim")):
    os.system("mv ~/.vim/colors/mymorning.vim ~/.vim/colors/mymorning.vim.old")
    print "Previous .vim/colors/mymorning.vim moved to .vim/colors/mymorning.vim.old"
  os.system("mkdir -p ~/.vim/colors")
  os.system("ln -s "+mysvn+"/scripts/my_morning.vim ~/.vim/colors/mymorning.vim")
