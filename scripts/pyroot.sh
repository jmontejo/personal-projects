#!/usr/bin/env bash
python -i -c "
print 'import ROOT, sys, os'
import ROOT, sys, os
if len(sys.argv)>1:
    rfile = ROOT.TFile.Open(sys.argv[1])
    print 'rfile=',rfile
" $1

