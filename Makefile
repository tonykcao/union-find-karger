CXX = g++
CXXFLAGS = -O2
# CXXFLAGS = -pg -Wall -Wvla -Werror -Wno-error=unused-variable
PROGRAMS = mincut
MC = mincut

all: $(PROGRAMS)

mincut: mincut.cpp
	$(CXX) $(CXXFLAGS) $(MC).cpp -o $(MC).o

clean:
	rm -f *.pdf *.aux *.log *.o

.PHONY: all clean

