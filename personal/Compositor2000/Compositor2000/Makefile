CXX           =
ObjSuf        = o
SrcSuf        = cpp
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o 
LinkDings     = -c

# Here we setup the list of compiler flags and libraries specific for ROOT
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs) 
ROOTGLIBS    := $(shell root-config --glibs)

# What follows are architecture dependent setups
# You can ignore most if not all of this part

CXX           = g++
CXXFLAGS      = -O -Wall -fPIC -g
LD            = g++
LDFLAGS       = -O 
SOFLAGS       = -shared


# Now we are ready to compile!


CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS) #-lMinuit2
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------

MAIN  = main$(ExeSuf)
MAINO = main.$(ObjSuf)

THEFITTER  = theFitter$(ExeSuf)
THEFITTERO = theFitter.$(ObjSuf)

FITFUNCTION  = fitFunction$(ExeSuf)
FITFUNCTIONO = fitFunction.$(ObjSuf)

NUISANCEFUNCTION  = nuisanceFCN$(ExeSuf)
NUISANCEFUNCTIONO = nuisanceFCN.$(ObjSuf)

PARSER  = Parser$(ExeSuf)
PARSERO = Parser.$(ObjSuf)

INFOCONT  = infoContainer$(ExeSuf)
INFOCONTO = infoContainer.$(ObjSuf)

SUMOUT  = outSum$(ExeSuf)
SUMOUTO = outSum.$(ObjSuf)

GMAP  = gmap$(ExeSuf)
GMAPO = gmap.$(ObjSuf)


#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

#all:              $(INFOCONT) $(GMAP) $(SUMOUT) $(PARSER) $(FITFUNCTION) $(THEFITTER) $(FITTERMAIN) $(NUISANCEFUNCTION)
all:              main 

#$(FITTERMAIN):    $(FITTERMAINO) $(THEFITTERO) $(FITFUNCTIONO) $(NUISANCEFUNCTION) $(INFOCONTO) $(GMAPO) $(SUMOUTO) $(PARSERO) fileNameDefs.h
#			$(LD) $(LDFLAGS) $(FITTERMAINO) $(LIBS) $(OutPutOpt) $(FITTERMAIN) $(INFOCONTO) $(GMAPO) $(SUMOUTO) $(PARSERO) $(FITFUNCTIONO) $(NUISANCEFUNCTIONO) $(THEFITTERO)
#			@echo "$@ done"
main:    main.o Monitor.o Partitura.o Solucion.o
			$(LD) $(LDFLAGS) main.o $(LIBS) $(OutPutOpt) main Monitor.o Partitura.o Solucion.o
			@echo "$@ done"

Monitor:	Monitor.o
			@echo "$@ done"

Partitura.o:	Compas.o Coste.o Instrumento.o Posicion.o Solucion.o
			$(CXX) $(CXXFLAGS) -c Partitura.cpp
			@echo "$@ done"

Solucion:	Solucion.o 
			@echo "$@ done"

Compas.o:	Nota.o
			@echo "$@ done"

Nota:		Nota.o
			@echo "$@ done"

Instrumento.o:	Nota.o
			@echo "$@ done"

Posicion:	Posicion.o
			@echo "$@ done"

Coste:		Coste.o
			@echo "$@ done"
#$(THEFITTER):    $(THEFITTERO) $(PARSERO) $(SUMOUTO) $(GMAPO)
#			@echo "$@ done"

clean:
		@rm -f *.$(ObjSuf) core

distclean:      clean
		@rm -f *.$(ObjSuf) *.def *.exp \
		   *.root *.ps *.so .def so_locations

.SUFFIXES: .$(SrcSuf)

###


.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

