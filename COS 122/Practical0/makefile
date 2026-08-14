#26080975
CXX = g++
CXXFLAGS = -Wall -std=c++11

all: process_main

process_main: process_main.cpp
	$(CXX) $(CXXFLAGS) -o process_main process_main.cpp

run: process_main
	./process_main

clean:
	rm -f process_main