CXX := g++
SRCDIR := src
BUILDDIR := build
PREFIX := /usr/local
TARGET := bin/qbri-rna bin/qbri-wes bin/checkFile

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f ! -name ._*.$(SRCEXT) -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
TOBJECTS := $(patsubst bin/%, $(BUILDDIR)/%.o,$(TARGET) )
CXXFLAGS := -g  -std=c++11 # -Wall
LIB := -pthread  -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt #-L/usr/local/Cellar/boost/1.64.0_1/ #-lmongoclient 
#LIB := -pthread  -L lib -lboost_thread -lboost_filesystem -lboost_system #-L/usr/local/Cellar/boost/1.64.0_1/ #-lmongoclient 
INC := -I include #-I/usr/local/Cellar/boost/1.64.0_1/include

all: $(TARGET)

$(TARGET): $(OBJECTS)
	echo " Linking...";
	echo "\n $(TOBJECTS)) \n";
	echo " $(CXX) $(filter-out $(TOBJECTS), $^) $(patsubst bin/%, $(BUILDDIR)/%.o, $@ ) -o $@ $(LIB)"; $(CXX) $(filter-out $(TOBJECTS), $^) $(patsubst bin/%, $(BUILDDIR)/%.o, $@ ) -o $@ $(LIB);

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(SRCDIR)/config.h $(SRCDIR)/version.h
	mkdir -p $(BUILDDIR);
	echo "$<";
	echo " $(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<


.PHONY: clean install
 
clean:
	echo " Cleaning..."; 
	echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

install: $(filter-out bin/checkFile,  $(TARGET))
	install -v -t $(PREFIX)/bin -D $^  

# Tests
# tester:
#   $(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester


