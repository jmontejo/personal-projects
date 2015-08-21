import time

class log:

  def hasResource(self, content):
    res_keys = ["http","html","indico.cern","/nfs","cdsweb.cern"]
    for key in res_keys:
      if content.find(key) > -1:
        return True
    return False

  def parseTags(self, tags):
    taglist = []
    try:
      ini = tags.index("'")
      end = tags.index("'",ini+1)
      while ini > -1 and end > -1:
        taglist.append(tags[ini+1:end].strip())
        tags = tags[end+1:]
        ini = tags.find("'")
        if ini > -1:
          end = tags.find("'",ini+1)
    except:
      print "parseTags exception"
      print tags
      taglist = [""]
    return taglist

  def parseLog(self, text):
    try:
      poslog = text.index("log:")
      posid = text.index("ID:")
      postag = text.index("tag:")
      possummary = text.index("summary:")
      poscontent = text.index("content:")
      self.date = text[poslog+4:posid].strip()
      self.ID = int(text[posid+3:postag].strip())
      self.tag = self.parseTags(text[postag+4:possummary])
      self.summary = text[possummary+8:poscontent].strip()
      self.content = text[poscontent+8:].strip()
      if 'hasResource' not in self.tag and self.hasResource(self.content):
        self.tag.append('hasResource')
      if self.content == "":
        self.valid = False
      else:
        self.valid = True
    except:
      print "parseLog exception"
      print text
      self.valid = False

  def __init__(self, text=None, id=0):
    self.ID  = id
    self.tag = []
    if text is not None and text != "dummy":
      self.parseLog(text)
    else:
      self.valid = True
      self.summary = ""
      self.tag  = [""]
      self.date = time.strftime("%Y-%m-%d",time.gmtime())
      self.content = ""

  def check(self, field, condition):
    if field=="tag":
      if condition in self.tag:
        return True
      return False
    elif field=="date":
      return self.date == condition
    elif field=="summary":
      if self.summary.find(condition) > -1:
        return True
      else:
        return False
    elif field=="content":
      if self.content.find(condition) > -1:
        return True
      else:
        return False
    else:
      print "This field does not exist: " + field
      quit()
  
  def isValid(self):
    return self.valid 

  def tostring(self):
    string = "log: "+self.date+"\n"
    string += "ID: "+str(self.ID)+"\n"
    string += "tag: "+str(self.tag)+"\n"
    string += "summary: "+str(self.summary)+"\n"
    string += "content: "+self.content+"\n"
    return string

  def show(self):
    print "------"
    print self.ID
    print self.date
    print self.tag
    print self.summary
    print self.content
    print "------"
