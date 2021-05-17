#! /usr/bin/python2.7

import os,sys,glob,shutil,re
from optparse import OptionParser

class pattern:

  def __init__(self,string_pattern):
    string_pattern = os.path.normpath(string_pattern)
    self.original = string_pattern
    self.blocks = []
    prepos = 0
    pos = string_pattern.find("*") 
    while pos > -1:
      self.blocks.append(string_pattern[prepos:pos])
      prepos = pos+1
      pos = string_pattern.find("*",prepos)
    self.blocks.append(string_pattern[prepos:])

  def compatible(self,other):
    if len(self.blocks) != len(other.blocks):
      return False
    else:
      return True

class renamer:

  def __init__(self,in_p,out_p,mapwildcard):
    self.in_blocks    = in_p.blocks
    self.in_blocks_re = in_p.blocks[:]
    if self.in_blocks_re[0] == '' : self.in_blocks_re[0]  = "^"
    if self.in_blocks_re[-1] == '': self.in_blocks_re[-1] = "$"
    self.out_blocks    = out_p.blocks
    self.mapwildcard = mapwildcard

  def getrename(self,thestr):
    pos = []
    prepos = 0
    for in_block in self.in_blocks_re:
      pos.append(re.search(in_block,thestr[prepos:]).start()+prepos)
      if in_block != "^" and in_block != "$":
        prepos = pos[-1]+len(in_block)
    pos.append(len(thestr))

    out = []
    for i in range(0,len(self.in_blocks)):
      if self.in_blocks[i] == '':
        out.extend( [self.out_blocks[i], thestr[pos[i]:pos[i+1]]] )
      else:
        out.extend( [self.out_blocks[i], thestr[pos[i]:pos[i+1]].replace(self.in_blocks[i],"")])
    out = "".join([out[self.mapwildcard.get(i,i)] for i in range(len(out))])
    
    return out

if __name__=="__main__":

  parser = OptionParser()
  parser.add_option ("-v","--verbose",
                     help="Turn on verbose printout",
                     dest="verbose",
                     action="store_true",
                     default=False)
  parser.add_option ("-f","--force",
                     help="Overwrite files without asking",
                     dest="force",
                     action="store_true",
                     default=False)
  parser.add_option ("-t","--test",
                     help="Print the renaming but don't perform it",
                     dest="test",
                     action="store_true",
                     default=False)
  parser.add_option ("-s","--skip",
                     help="Don't ask for existing files, skip them",
                     dest="skip",
                     action="store_true",
                     default=False)
  parser.add_option ("-c","--copy",
                     help="Make a copy of the files with the new name instead of renaming",
                     dest="copy",
                     action="store_true",
                     default=False)
  parser.add_option ("-l","--link",
                     help="Make a *soft* link of the files with the new name instead of renaming",
                     dest="link",
                     action="store_true",
                     default=False)
  parser.add_option ("-F","--follow-link",
                     help="If the original file is a soft link, the destination is not a link but a file",
                     dest="follow",
                     action="store_true",
                     default=False)
  parser.add_option ("-m","--map",
                     help="Reorder the wildcards in the output according to this mapping. Format x:x[:x ...]",
                     dest="mapwildcard",
                     default="")
  
  options,args = parser.parse_args()
  verbose = options.verbose
  force   = options.force  
  test    = options.test
  skip    = options.skip
  copy    = options.copy
  link    = options.link
  follow  = options.follow

  if len(args) != 2:
    print "Usage: rename in_pattern out_pattern"
    sys.exit(1)
  if link and copy:
    print "Incompatible options: copy+link"
    sys.exit(1)

  in_pattern = pattern(args[0])
  out_pattern = pattern(args[1])
  if not in_pattern.compatible(out_pattern):
    print "The patterns are not compatible, same number of wildcards '*' expected"
    sys.exit(1)

  if options.mapwildcard:
      try:
          mapwildcard       = dict((1+2*k,1+2*int(v)) for k,v in enumerate(options.mapwildcard.split(":"))) 
      except Exception:
          print "Badly formed mapping, needs to be integers in the format x:x[:x ...]"
          sys.exit(1)
  else:
      mapwildcard = dict()

  in_list = glob.glob(args[0])
  myRenamer = renamer(in_pattern,out_pattern,mapwildcard)
  for item in in_list:
    out = myRenamer.getrename(item)
    if os.path.exists(out) and skip:
      continue
    if os.path.exists(out) and not force:
      key = raw_input(out+" exists, overwrite? y/[n]")
      if key is not "y": continue
    if link:
      if os.path.lexists(out):
        os.remove(out)
    print item+" --> "+out
    if not test:
      if follow and (copy or link) and os.path.islink(item):
        item = os.path.abspath(os.path.join(os.path.dirname(item),os.readlink(item)))

      if copy:
				if os.path.isdir(item):
					shutil.copytree(item,out)
				else:
					shutil.copy(item,out)
      elif link: os.symlink(os.path.abspath(item), out)
      else:      shutil.move(item,out)

