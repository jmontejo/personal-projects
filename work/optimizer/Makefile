FILES :=
FILES += optimizer
FILES += computeVar
FILES += variablePool
FILES += Options
FILES += GeneticAlgorithm

OBJS  := $(addsuffix .o, $(FILES))

all: optimizer

#lib: computeVar.cxx computeVar.h
#	g++ -Wall -c `root-config --cflags` -o computeVar.so computeVar.cxx computeVar.h
  

optimizer: $(OBJS)
	g++ --std=c++0x -m64 -Wall $(OBJS) `root-config --cflags --libs` -lMinuit -o optimizer

optimizer.o: optimizer.cxx
	g++ --std=c++0x -m64 -Wall -c `root-config --cflags` -o $@ $<

%.o: %.cxx %.h
	g++ --std=c++0x -m64 -Wall -c `root-config --cflags` -o $@ $<

clean:
	rm *.o optimizer AutoDict* *~ *.gch
