#! /usr/bin/env python2.7
"""
pdfize.py
  Makes a pdf view of the contents of the given folder and places it in it.
  Depending on the format of the files (given by the extension) it can display images,
  print text and compile tex files.
  The script relies on pdflatex being installed <- to-do, check if pdflatex is available

  -> need to include header packages that include eps and png files
"""
import os, sys, time
from optparse import OptionParser

def pdfize(folder,outfile,options):
  texfilename = os.path.join(folder,os.path.basename(folder)+".tex")
  while os.path.exists(texfilename): texfilename += ".tex"
  #to avoid overwritting pdf files called as the texfile
  while os.path.exists(texfilename[:-3]+"pdf"): texfilename += ".tex" 
  print "pdfize: ",folder,"->",outfile
  with open(texfilename,"w") as texfile:
    header(texfile,options.docclass)
    toclean = processFolder(folder,texfile,texfilename,options,outfile)
    finish(texfile)
  ok = compile_clean(texfilename,outfile,folder,toclean,options.verbose)
  if not ok:
    print "Output not created, empty?"

def processFolder(folder,texfile,texfilename,options,outfile,level=0):
  
  toclean = []
  images, files, folders = folderContent(folder,texfilename,outfile,options.keys,options.allkeys,options.veto)
  if options.verbose:
    print folder
    print " "*level+"images  :",images
    print " "*level+"files   :",files
    print " "*level+"folders :",folders
  if files:  processFiles(files,texfile,options.block)
  if images: toclean += processImages(images,texfile,options.grid,options.block)
  if options.recursive:
    for f in folders: toclean += processFolder(f,texfile,texfilename,options,outfile,level+2)
  return toclean

def header(texfile,docclass):
  texfile.write("\\documentclass{"+docclass+"} \n")
  texfile.write("\\usepackage[a4paper,left=3cm,right=2cm,top=2.5cm,bottom=2.5cm]{geometry}\n")
  texfile.write("\\usepackage{graphicx}\n")
  texfile.write("\\begin{document}\n")
  
def finish(texfile):
  texfile.write("\\end{document}\n")

def folderContent(folder,texfilename,outfile,keys,allkeys,vetos):
  items = os.listdir(folder)
  images, files, folders = [],[],[]
  extensions = ("png","eps","ps","jpg","gif","tiff","pdf")
  for item in sorted([os.path.join(folder,x) for x in items ]):
    if item == outfile: continue
    if os.path.isdir(item):
      folders.append(item)
    else:
      if keys    and not any([ key in item for  key in keys   ]): continue
      if allkeys and not all([akey in item for akey in allkeys]): continue
      if any([veto in item for veto in vetos]): continue
      for ext in extensions:
        if item.endswith(ext):
          images.append(item)
          break
      else:
        if item != texfilename:
          files.append(item)
  return (images, files, folders)

def processFiles(files,texfile,blocks):
  extensions = ("txt","tex")
  for block in blocks:
    for file in files:
      if not block in file: continue
      for ext in extensions:
        if file.endswith(ext):
          texfile.write("\n\n"+file+": \n\n")
          if ext=="tex":
            texfile.write(checkTex(file)+"\n\n")
          else:
            texfile.write(open(file).read()+"\n\n")
          break

def checkTex(filename):
  begindoc = "\\begin{document}"
  enddoc = "\\end{document}"
  with open(filename) as f:
    content = f.read()
    if begindoc in content:
      content = content[content.find(begindoc)+len(begindoc):]
    if enddoc in content:
      content = content[:content.find(enddoc)]
  return content

def processImages(files,texfile,grid,blocks):
  width = round(1./grid,2)
  toclean = set()
  for block in blocks:
    texfile.write("\\newpage \n")
    texfile.write("{\large "+block+"}\n")
    texfile.write("\\begin{center} \n")
    texfile.write("\\begin{tabular}{"+("c"*grid)+"} \n")
    caption = ""
    for i,file in enumerate(files):
      if not block in file: continue
      file = os.path.relpath(file)
      if file.endswith(".eps"):
        toclean.add(file.replace(".eps","-eps-converted-to.pdf"))
      texfile.write("\\includegraphics[width="+str(width)+"\\textwidth]{"+file+"}")
      caption += os.path.basename(file).replace("_","\\_")
      if i==len(files)-1: 
        texfile.write("\\\\ \n")
        caption += " \\\\ \n"
        texfile.write(caption)
        caption = ""
      elif i%grid==grid-1:
        texfile.write("\\\\ \n")
        caption += " \\\\ \n"
        texfile.write(caption)
        caption = ""
        texfile.write("\\end{tabular} \n")
        texfile.write("\\begin{tabular}{"+("c"*grid)+"} \n")
      else:         
        texfile.write(" & \n")
        caption += " & "
    texfile.write("\\end{tabular} \n")
    texfile.write("\\end{center} \n")
  return toclean

def compile_clean(texfilename,outfile,outfolder,toclean, verbose):
  base = texfilename[:-3]
  #cmd = "pdflatex  -output-directory "+outfolder+" -interaction batchmode "+texfilename
  if verbose:
    cmd = "pdflatex -output-directory "+outfolder+" -halt-on-error "+texfilename
  else:
    cmd = "pdflatex -output-directory "+outfolder+" -halt-on-error > /dev/null "+texfilename
  with open(texfilename) as f:
    if not f.read():
      print "Empty tex file, skipping"
      os.remove(texfilename)
    else:
      status = os.system(cmd)
      if status!=0:
        print "Compilation failed, check tex file"
        os.system("rm "+base+"{log,aux}")
      else:
        os.system("rm "+base+"{log,tex,aux}")
        defout = base+"pdf"
        if defout!=outfile:
          os.system("mv "+defout+" "+outfile)
  try:
    for item in toclean:
      os.remove(item)
  except OSError: pass
  return os.path.exists(outfile)

if __name__ == "__main__":
  parser = OptionParser() 
  parser.add_option ("-v","--verbose", 
                     help="Print files as they are processed",
                     dest="verbose", 
                     action="store_true", 
                     default=False) 
  parser.add_option ("-r","--recursive", 
                     help="Iterate recursively over all the subfolders", 
                     dest="recursive", 
                     action="store_true", 
                     default=False) 
  parser.add_option ("-o","--output-file", 
                     help="Specify the output file. Default is foldername.pdf",
                     dest="outfile", 
                     default="") 
  parser.add_option ("-O","--output-folder", 
                     help="Specify the output folder. Default is the folder over which it is running. \
                           This option is incompatible with -o when running over more than one folder. The output files will be the folder names.",
                     dest="outfolder", 
                     default="") 
  parser.add_option ("-d","--document-class", 
                     help="Specify the documentclass tag for the texfilename. Default is article",
                     dest="docclass", 
                     default="article") 
  parser.add_option ("-g","--grid", 
                     help="Specify number of columns in which the images should be displayed. Default is 1",
                     dest="grid", 
                     default=1) 
  parser.add_option ("-b","--block", 
                     help="Specify a key which will be used to group results",
                     dest="block", 
                     action="append",
                     default=[""]) 
  parser.add_option ("-k","--keys", 
                     help="Use only files matching at least one key (space separated list with quotes)",
                     dest="keys", 
                     default="") 
  parser.add_option ("-K","--allkeys", 
                     help="Use only files matching all keys (space separated list with quotes)",
                     dest="allkeys", 
                     default="") 
  parser.add_option ("-V","--veto", 
                     help="Use only files *not* matching key (space separated list with quotes)",
                     dest="veto", 
                     default="whatthefuckisthis") 
  parser.add_option ("-u","--use-package", 
                     help="Specify packages to be included in the main tex file",
                     dest="package", 
                     nargs = "+",
                     default=[]) 
  
  options,args = parser.parse_args()
  options.block.reverse()
  options.keys  = options.keys.split()
  options.allkeys  = options.allkeys.split()
  options.veto  = options.veto.split()
  
  if not args:
    print "Usage pdfize [options] folder"
    sys.exit(1)

  try: options.grid = int(options.grid)
  except ValueError: 
    print "Invalid value for grid:",options.grid
    print "Setting it to default grid=1"
    options.grid = 1

  if options.outfolder and options.outfile and len(args) > 1:
    print "WARNING - An output folder and name has been specified, but you are running over more than one folder"
    print "          To avoid overwritting of the files the output will be renamed to the folder name"
    options.outfile = ""
  
  for folder in args:
    if not os.path.isdir(folder): 
      folder,"is not a folder, skipping"
      continue
    if not options.outfolder: outfolder = folder
    else:                     outfolder = options.outfolder
    folder = os.path.abspath(folder)
    outfolder = os.path.abspath(outfolder)
    if not options.outfile: outfile = os.path.basename(folder)+".pdf"
    else:                   outfile = options.outfile
    outfile = os.path.join(outfolder,outfile)
    pdfize(folder,outfile,options)
  
