from logs import *
import subprocess, os

class logDB:

  home = os.getenv("MYSVN")
  DBstorage=home+"/scripts/logger/myLogDB.txt"
  tempfile="dummy.temp"
  header=""

  def parse(self, xml):
    ini = xml.find("log:")
    end = xml.find("log:",ini+1)
    self.logDict = dict()
    while ini > -1:
      temp = log(xml[ini:end])
      if temp.isValid():
        self.logDict[temp.ID] = temp
      xml = xml[end:]
      ini = xml.find("log:")
      end = xml.find("log:",ini+1)
    return self.logDict

  def __init__(self, xmlfile=None):
    self.logDict = dict()
    self.header = "#Log DB"
    if xmlfile is not None:
      self.header += ": " + xmlfile
      fin = open(xmlfile,'r')
      self.logDict = self.parse(fin.read())
      fin.close()

  def select(self, field, condition, addDummy=False):
    selected = logDB()
    selected.header += " selected from: " + field + " = " + condition
    for l in self.logDict.values():
      if l.check(field,condition):
        selected.logDict[l.ID] = l
    if addDummy is True:
      temp = log("dummy",self.logDict.keys()[-1]+1)
      selected.logDict[temp.ID]=temp
    return selected

  def edit(self):
    self.dump(self.tempfile)
    subprocess.call(["/usr/bin/vim",self.tempfile])
    fin = open(self.tempfile,'r')
    self.logDict = self.parse(fin.read())
    fin.close()
    subprocess.call(["/bin/rm",self.tempfile])

  def merge(self, mod):
    for id, modLog in mod.logDict.iteritems():
      self.logDict[id] = modLog

  def dump(self, file):
    sorted = []
    sorted = self.logDict.keys()
    sorted.sort()
    sorted.reverse()
    fout = open(file,'w')
    fout.write(self.header+"\n\n")
    for key in sorted:
      fout.write(self.logDict[key].tostring()+"\n")
    fout.close()

  def getLogs(self):
    return self.logDict
