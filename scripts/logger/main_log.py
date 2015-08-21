import os
from logDB import *
from optparse import OptionParser
from webizer import *

def str_today():
  return time.strftime("%Y-%m-%d",time.localtime())


if __name__=="__main__":

  parser = OptionParser()
  parser.add_option("-t", "--tag",
                    dest="c_tag",
                    help="Select logs with the given tag",
                    default="")
  parser.add_option("-T", "--text",
                    dest="c_text",
                    help="Select logs which contain the given text",
                    default="")
  parser.add_option("-d", "--date",
                    dest="c_date",
                    help="Select logs from the selected date",
                    default=str_today())
  parser.add_option ("-v","--verbose",
                     help="Turn on verbose printout",
                     dest="verbose",
                     action="store_true",
                     default=False)
  
  options,args = parser.parse_args()
  verbose = options.verbose
  c_tag   = options.c_tag
  c_text  = options.c_text
  c_date  = options.c_date

  addDummy = False
  if c_tag != "":
    field     = "tag"
    condition = c_tag
  elif c_text != "":
    field     = "text"
    condition = c_text
  else:
    field     = "date"
    condition = c_date
    if c_date == str_today():
      addDummy = True

  mylogDB = logDB(logDB.DBstorage)
  selected = mylogDB.select(field,condition,addDummy)
  selected.edit()
  mylogDB.merge(selected)
  mylogDB.dump(logDB.DBstorage)

  mwebizer = webizer()
  mwebizer.dump(mylogDB)

  home = os.getenv("HOME")
  os.system(home+"/scripts/logger/uploadWeb.sh &> /dev/null &")

