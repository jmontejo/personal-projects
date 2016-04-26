import math

files = ["Analysis/Figures_stop2/yieldsTable_postfit.tex","Analysis/Figures_stop2/yieldsTable_prefit.tex"]

def doRound(tyield,error):
  if error <= 0.: return  "%.2f \\pm %.2f" % (tyield,error)

  if int(math.floor(math.log10(tyield))) > int(math.floor(math.log10(error))):
    if tyield > 95 : tyield = str(int(round(float("%.3g" % tyield),0)))
    else           : tyield = str(float("%.3g" % tyield))
  else:
    if tyield > 9.5: tyield = str(int(round(float("%.2g" % tyield),0)))
    else           : tyield = str(float("%.2g" % tyield))
  if error > 9.5: error = str(int(round(float("%.2g" % error),0)))
  else          : error = str(float("%.2g" % error))
  posttoken = "%s \\pm %s" % (tyield,error)
  return posttoken


for file in files:
  with open(file) as infile:
    with open(file.replace(".tex","_rounded.tex"),"w") as outfile:
      for line in infile:
        if "\\pm" in line:
          line = line.replace("$"," $ ").replace("&"," & ")
          tokens = line.split()
          posttokens = []
          tyield = -1
          print line
          for token in tokens:
            try:
              num = float(token)
              if "Data" in line: posttoken = token
              elif tyield<0: tyield = num
              else:
                print line,tyield,num
                posttoken = doRound(tyield,num)
                posttokens.append(posttoken)
                tyield = None
            except ValueError,e:
              print e
              if tyield <0:
                posttokens.append(token)
          line = " ".join(posttokens)+"\n"
          line = line.replace("$ ","$").replace(" $","$")
        outfile.write(line)
