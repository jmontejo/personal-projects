"""
This manager is intended to be run as a cronjob. The default behaviour is producing a log file.
Additionally, this log can be emailed, and if desired automatic actions can be performed.

The manager recives one folder to monitor and places the log on the defined place or the top level if undefined.
"""

import os, sys
from optparse import OptionParser

def main():

  parser = OptionParser() 
  parser.add_option ("-f","--folder", 
                     help="Folder to monitor", 
                     dest="folder", 
                     default="") 
  parser.add_option ("-o","--output", 
                     help="Full path to the output log. Default is folder_to_monitor/log.txt", 
                     dest="log", 
                     default="") 
  parser.add_option ("-a","--auto-folders", 
                     help="Folders that will be updated and comitted automatically", 
                     dest="auto", 
                     default="")
  parser.add_option ("-e","--extension", 
                     help="Skip files with the given extensions", 
                     dest="extension", 
                     default="")
  parser.add_option ("-k","--key", 
                     help="Skip files which contain one of the given keys", 
                     dest="keys", 
                     default="")

  options,args = parser.parse_args() 
  folder    = os.path.abspath(options.folder)
  logfile   = options.log
  auto      = options.auto
  extension = options.extension
  keys      = options.keys

  if not logfile: logfile = os.path.join(folder,"svnmonitor_log.txt")
  log       = open(logfile,"w")

  if auto:
    log.write("AUTO: Updating "+auto+"\n")
    stdout = os.popen("svn up --force "+auto).read()
    log.write(stdout)
    log.write("AUTO: Adding "+auto+"\n")
    stdout = os.popen("svn add --force "+auto).read()
    log.write(stdout)
    log.write("AUTO: Committing "+auto+"\n")
    stdout = os.popen("svn ci -m autocommit "+auto).read()
    log.write(stdout)

  stdout = os.popen("svn status "+folder).read()
  svnitems = dict()
  for line in stdout.split("\n"):
    if not line: continue
    tokens = line.split()
    if len(tokens)==2:
      for ext in extension:
        if tokens[-1].endswith(ext): break
      else:
        for key in keys:
          if key in tokens[-1]: break
        else:
#          svnitems[tokens[-1]] = tokens[0]
          item = tokens[-1]
          status = tokens[0]
          log.write(status+"\t"+item+"\n")
    else:
      log.write("Weird line from svn status "+folder+": "+line+"\n")

#  for item, status in svnitems.iteritems():
#    log.write(status+"\t"+item+"\n")

  log.close()

if __name__ == "__main__": main()
