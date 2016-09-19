#!/usr/bin/env python

from DQUtils import fetch_iovs, process_iovs, process_iovs_changed

def readlumi(run):
    # Online calibrated lumi
    lumi = fetch_iovs('COOLONL_TRIGGER/CONDBR2::/TRIGGER/LUMI/OnlPrefLumi',
                      tag='HEAD',
                      runs=run)
    
    # Offline calibrated lumi
    #lumi = fetch_iovs('COOLOFL_TRIGGER/CONDBR2::/TRIGGER/OFLLUMI/OflPrefLumi',
    #                  tag='OflPrefLumi-RUN2-UPD4-04',
    #                  runs=run)

    # Lumi format documented here:
    # https://twiki.cern.ch/twiki/bin/viewauth/AtlasComputing/CoolOnlineData#Folder_TRIGGER_OFLLUMI_OflPrefLu
    for since, until, (l,) in process_iovs(lumi):
        print since.lumi, l.LBAvInstLumi, l.LBAvEvtsPerBX

readlumi(284213)

