hist2workspace master.xml
root -b -l -q fit.C
python doAllPlotsInList.py --variables hist --yields False --ext "eps png"
