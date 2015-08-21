from logDB import *
import htmltemplate
import string

class webizer:

  home = os.getenv("MYSVN")
  htmlStorage=home+"/scripts/logger/web_content/"

  def dump(self, logDB):
    self.doPage(logDB)
    tagList = []
    for key, log in logDB.getLogs().iteritems():
      for tag in log.tag:
        if tag not in tagList:
          tagList.append(log)
          selected = logDB.select("tag",tag)
          self.doPage(selected,tag)

  def doPage(self,logDB,outfile="index.html"):
    if outfile != "index.html":
      outfile = "logger_"+outfile+".html"

    fout = open(self.htmlStorage+outfile,'w')
    fout.write(htmltemplate.indexheader1)
    logDict = logDB.getLogs()
    for i in range(len(logDict)):
      fout.write("#tbl"+str(i))
      if i != (len(logDict)-1):
        fout.write(",")
    fout.write(htmltemplate.indexheader2)
    for i in range(len(logDict)):
      fout.write("#lnk"+str(i))
      if i != (len(logDict)-1):
        fout.write(",")
    fout.write(htmltemplate.indexheader3)

    tagList = []
    for key in logDict.keys():
      for t in logDict[key].tag:
         if t not in tagList:
           tagList.append(t)
    tagList.sort()
    fout.write(self.doTagHeader(tagList))

    i=0
    sorted = []
    sorted = logDict.keys()
    sorted.sort()
    sorted.reverse()
    for key in sorted:
      code = self.doLog(logDict[key],str(i))
      i+=1
      fout.write(code)

    fout.write(htmltemplate.end)
    fout.close()

  def doLog(self,log,n):
    code  = htmltemplate.log1
    code += log.date
    code += htmltemplate.log2
    for tag in log.tag:
      code += self.doTag(tag)
    code += self.doTag()
    code += htmltemplate.log3
    code += self.doInputTag(n)
    code += htmltemplate.log4
    code += log.summary
    code += htmltemplate.log5
    code += str(n)
    code += htmltemplate.log6
    code += self.doContent(log.content)
    code += htmltemplate.log7
    return code

  def doTag(self,tag=None):
    if tag is None:
      return "<a href=\"index.html\" >All</a>\n"
    else:
      return "<a href=\"logger_"+tag+".html\" >"+tag+"</a>, \n"

  def doTagHeader(self,tagList):
    code  = htmltemplate.log1
    code += "Tag list"
    code += htmltemplate.log2
    code += htmltemplate.log3
    code += htmltemplate.log4
    for t in tagList:
      code += self.doTag(t)
    code += self.doTag()
    code += htmltemplate.log5
    code += htmltemplate.log6
    code += htmltemplate.log7
    return code
   
  def doInputTag(self,n):
    return "<input id=\"lnk"+n+"\" type=\"button\" value=\"[+] Expand\" onclick=\"toggle_visibility('tbl"+n+"','lnk"+n+"');\">"

  def doContent(self,content):
    res_keys = ["http","indico.cern","cdsweb.cern"]
    lines = content.splitlines(True)
    c = ""
    for line in lines:
      for key in res_keys:
        ini = line.find(key)
        if ini > -1:
          end = line.find(" ",ini) # from pos->end
          if end == -1:
             end = len(line)
          while line[end-1] in string.punctuation:
             end = end-1
          link = line[ini:end]
          modline = line[:ini]+"<a href=\""+link+"\">"+link+"</a>"+line[end:]
          line = modline
          break
      c += line+"<br>\n"
    return c
#  <tr height="1"> 
#   <td bgcolor="#727272" colspan="3"></td> 
#  </tr> 
#  <tr bgcolor="#EEEEEE" height="15"> 
#   <td><strong>Date</strong></td> 
#   <td>tags with links</td> 
#   <td bgcolor="#EEEEEE" align="right"><input id="lnk1" type="button" value="[+] Expand" onclick="toggle_visibility('tbl1','lnk1');"></td> 
#  </tr> 
#  <tr bgcolor="#EEEEEE" height="15"> 
#   <td colspan="3">here comes the text before the expansion</td> 
#  </tr> 
#  <tr> 
#   <td colspan="3"> 
#    <table width="100%" border="0" cellpadding="4" cellspacing="0" id="tbl1"> 
#     <tr> 
#      <td colspan="3">Short summary which describes Project 1.</td> 
#     </tr> 
#    </table> 
#   </td> 
#  </tr> 

