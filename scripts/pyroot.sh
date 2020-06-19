#!/usr/bin/env bash
#Open python in interactive session, import ROOT and open optional file

python -i -c "
print 'import ROOT, sys, os'
import ROOT, sys, os
if len(sys.argv)>1:
    rfile = ROOT.TFile.Open(sys.argv[1])
    if not rfile:
        print 'Could not find file:',sys.argv[1]
    else:
        print 'rfile=',rfile
" $1 #optional file

