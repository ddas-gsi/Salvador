.EXPORT_ALL_VARIABLES:

.PHONY: clean all

BIN_DIR = $(HOME)/bin
LIB_DIR = $(HOME)/lib


ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTINC      := -I$(shell root-config --incdir)

CPP             = g++
CFLAGS		= -Wall -Wno-long-long -g -O3 $(ROOTCFLAGS) -fPIC -MMD

INCLUDES        = -I./inc -I$(TARTSYS)/include 
BASELIBS 	= -lm $(ROOTLIBS) $(ROOTGLIBS) -L$(LIB_DIR) -L$(TARTSYS)/lib -lXMLParser
LIBS  	=  $(BASELIBS) -lCommandLineInterface -lanaroot -lananadeko -lanacore -lanabrips -lanaloop -lanadali -lSalvador

LFLAGS		= -g -fPIC -shared
CFLAGS += -Wl,--no-as-needed
LFLAGS += -Wl,--no-as-needed 
CFLAGS += -Wno-unused-variable -Wno-write-strings

CLICFLAGS   = -g2 -O2 -fPIC
CLILFLAGS   = -g -fPIC -shared -Wl,--no-as-needed

LIB_O_FILES = build/FocalPlane.o build/FocalPlaneDictionary.o build/Beam.o build/BeamDictionary.o build/PPAC.o build/PPACDictionary.o build/DALI.o build/DALIDictionary.o 

O_FILES = build/Reconstruction.o build/Settings.o

USING_ROOT_6 = $(shell expr $(shell root-config --version | cut -f1 -d.) \>= 6)
ifeq ($(USING_ROOT_6),1)
	EXTRAS =  PPACDictionary_rdict.pcm FocalPlaneDictionary_rdict.pcm BeamDictionary_rdict.pcm DALIDictionary_rdict.pcm
endif

all: $(LIB_DIR)/libCommandLineInterface.so Metamorphosis FriedBacon BurningGiraffe Disintegration Persistence $(LIB_DIR)/libSalvador.so $(EXTRAS) 

Metamorphosis: Metamorphosis.cc $(LIB_DIR)/libSalvador.so build/Settings.o
	@echo "Compiling $@"
#	#$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) build/Settings.o -o $@ 
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) build/Settings.o -o $(BIN_DIR)/$@ 

FriedBacon: FriedBacon.cc $(LIB_DIR)/libSalvador.so build/Settings.o
	@echo "Compiling $@"
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) build/Settings.o -o $(BIN_DIR)/$@ 

BurningGiraffe: BurningGiraffe.cc $(LIB_DIR)/libSalvador.so
	@echo "Compiling $@"
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) -o $(BIN_DIR)/$@ 

Disintegration: Disintegration.cc $(LIB_DIR)/libSalvador.so
	@echo "Compiling $@"
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) -o $(BIN_DIR)/$@ 

Persistence: Persistence.cc $(LIB_DIR)/libSalvador.so $(O_FILES)
	@echo "Compiling $@"
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) $(O_FILES) -o $(BIN_DIR)/$@ 

Machine: Machine.cc $(LIB_DIR)/libSalvador.so $(O_FILES)
	@echo "Compiling $@"
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) $(O_FILES) -o $(BIN_DIR)/$@ 

Temptation: Temptation.cc $(LIB_DIR)/libSalvador.so $(O_FILES)
	@echo "Compiling $@"
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) $(O_FILES) -o $(BIN_DIR)/$@ 

Swans: Swans.cc $(LIB_DIR)/libSalvador.so
	@echo "Compiling $@"
	@$(CPP) $(CFLAGS) $(INCLUDES) $< $(LIBS) -o $(BIN_DIR)/$@ 

$(LIB_DIR)/libCommandLineInterface.so: build/CommandLineInterface.o  
	@echo "Making $@"
	@$(CPP) $(CLILFLAGS) -o $@ $^ -lc

$(LIB_DIR)/libSalvador.so: $(LIB_O_FILES) 
	@echo "Making $@"
	@$(CPP) $(LFLAGS) -o $@ $^ -lc

build/Reconstruction.o: src/Reconstruction.cc inc/Reconstruction.hh $(LIB_DIR)/libSalvador.so 
	@echo "Compiling $@"
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

build/BuildEvents.o: src/BuildEvents.cc inc/BuildEvents.hh $(LIB_DIR)/libSalvador.so 
	@echo "Compiling $@"
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

build/Calibration.o: src/Calibration.cc inc/Calibration.hh $(LIB_DIR)/libSalvador.so 
	@echo "Compiling $@"
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

build/%.o: src/%.cc inc/%.hh
	@echo "Compiling $@"
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

build/%Dictionary.o: build/%Dictionary.cc
	@echo "Compiling $@"
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) $(INCLUDES) -fPIC -c $< -o $@

build/%Dictionary.cc: inc/%.hh inc/%LinkDef.h
	@echo "Building $@"
	@mkdir -p build
	@rootcint -f $@ -c $(INCLUDES) $(notdir $^)

build/%Dictionary_rdict.pcm: build/%Dictionary.cc
	@echo "Confirming $@"
	@touch $@

%Dictionary_rdict.pcm: build/%Dictionary_rdict.pcm 
	@echo "Placing $@"
	@cp build/$@ $(LIB_DIR)

build/CommandLineInterface.o: src/CommandLineInterface.cc inc/CommandLineInterface.hh
	@echo "Building $@"
	@mkdir -p $(dir $@)
	@$(CPP) $(CLICFLAGS) $(INCLUDES) -c $< -o $@

doc:	doxyconf
	doxygen doxyconf


clean:
	@echo "Cleaning up"
	@rm -rf build doc
	@rm -f inc/*~ src/*~ *~
