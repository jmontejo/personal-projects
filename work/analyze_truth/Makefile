FILES := 
FILES += analyze_truth
FILES += Dataset
FILES += Output
FILES += SampleInfo
FILES += Reweight
FILES += EventShape
FILES += Sherpa2_xsec

OBJS  := $(addsuffix .o, $(FILES))


all: analyze_truth

analyze_truth: $(OBJS)
	g++ -Wall -O3 $(OBJS) -L/nfs/pic.es/user/j/jmontejo/scratch/FolderProduction_V23_stolenAndrea/edit_mc12b/RootCore/lib -lTopAnalysisBase -lTopD3PDTruthTools `root-config --cflags --libs` -o analyze_truth

analyze_truth.o: analyze_truth.cxx
	g++ -Wall -O3 -c -I/nfs/pic.es/user/j/jmontejo/scratch/FolderProduction_V23_stolenAndrea/edit_mc12b/RootCore/include `root-config --cflags` -o $@ $<

Sherpa2_xsec.o: Sherpa2_xsec.cxx Sherpa2_xsec.h
	g++ -Wall -c -I/nfs/pic.es/user/j/jmontejo/scratch/FolderProduction_V23_stolenAndrea/edit_mc12b/RootCore/include `root-config --cflags` -o $@ $<

%.o: %.cxx %.h
	g++ -Wall -O3 -c -I/nfs/pic.es/user/j/jmontejo/scratch/FolderProduction_V23_stolenAndrea/edit_mc12b/RootCore/include `root-config --cflags` -o $@ $<

clean:
	rm *.o analyze_truth
