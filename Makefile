# use g++ compiler
CXX=g++
CXXFLAGS?=-Wall -pedantic -std=c++11

# flag specifications for release and debug
RELEASEFLAGS?=$(CXXFLAGS) -O3
DEBUGFLAGS?=$(CXXFLAGS) -O0 -g #-pg

# executables
EXES=ct_distance ct_labels ct_rename

# compile
all: $(EXES)
ct_distance: compact_tree.h common.h ct_distance.cpp
	$(CXX) $(RELEASEFLAGS) -o ct_distance ct_distance.cpp
ct_labels: compact_tree.h common.h ct_labels.cpp
	$(CXX) $(RELEASEFLAGS) -o ct_labels ct_labels.cpp
ct_rename: compact_tree.h common.h ct_rename.cpp
	$(CXX) $(RELEASEFLAGS) -o ct_rename ct_rename.cpp
clean:
	$(RM) -r $(EXES) *.o
