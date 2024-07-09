# use g++ compiler
CXX=g++
CXXFLAGS?=-Wall -pedantic -std=c++11

# flag specifications for release and debug
RELEASEFLAGS?=$(CXXFLAGS) -O3
DEBUGFLAGS?=$(CXXFLAGS) -O0 -g #-pg

# executables
EXES=ct_labels

# compile
all: $(EXES)
ct_labels: compact_tree.h common.h ct_labels.cpp
	$(CXX) $(RELEASEFLAGS) -o ct_labels ct_labels.cpp
clean:
	$(RM) -r $(EXES) *.o
