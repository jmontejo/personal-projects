FILES :=
FILES += optimizer
FILES += computeVar
FILES += variablePool
FILES += Options

OBJS  := $(addsuffix .o, $(FILES))

all: optimizer

#lib: computeVar.cxx computeVar.h
#	g++ -Wall -c `root-config --cflags` -o computeVar.so computeVar.cxx computeVar.h
  

optimizer: $(OBJS)
	g++ -m64 -Wall $(OBJS) `root-config --cflags --libs` -lMinuit -o optimizer

optimizer.o: optimizer.cxx
	g++ -m64 -Wall -c `root-config --cflags` -o $@ $<

%.o: %.cxx %.h
	g++ -m64 -Wall -c `root-config --cflags` -o $@ $<

clean:
	rm *.o optimizer AutoDict* *~ *.gch
