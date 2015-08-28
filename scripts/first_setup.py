#! /usr/bin/python
#from optparse import OptionParser
import sys,os

if __name__ == "__main__":

  #parser = OptionParser() 
  #parser.add_option ("-L","--local", 
  #                   help="Set up all machines respect to the local one",
  #                   dest="all", 
  #                   action="store_true", 
  #                   default=False) 
  options,args = parser.parse_args()
  if len(args) != 1:
    print "Path to $MYSVN needed: first_setup.py $MYSVN"
    sys.exit(1)

  print "-- Link .bashrc .bash_profile"
  if os.path.exists(os.path.expandusr("~/.bashrc")):
    os.system("cp ~/.bashrc ~/.bashrc.old")
    print "Previous .bashrc copied to .bashrc.old
  os.symlink("$MYSVN/scripts/my_bashrc","~/.bashrc")
  if os.path.exists(os.path.expandusr("~/.bash_profile")):
    os.system("cp ~/.bash_profile ~/.bash_profile.old")
    print "Previous .bash_profile copied to .bas_profile.old
  os.symlink("$MYSVN/scripts/my_bash_profile","~/.bash_profile")

  print "-- Link .subversion/config"
  if os.path.exists(os.path.expandusr("~/.subversion/config")):
    os.system("cp ~/.subversion/config ~/.subversion/config.old")
    print "Previous .subversion/config copied to .subversion/config.old
  os.symlink("$MYSVN/scripts/my_subversion/config","~/.subversion/config")

  print "-- Link .vim/colors/mymorning.vim"
  if os.path.exists(os.path.expandusr("~/.vim/colors/mymorning.vim")):
    os.system("cp ~/.vim/colors/mymorning.vim ~/.vim/colors/mymorning.vim.old")
    print "Previous .vim/colors/mymorning.vim copied to .vim/colors/mymorning.vim.old
  os.symlink("$MYSVN/scripts/my_morning.vim","~/.vim/colors/mymorning.vim")
