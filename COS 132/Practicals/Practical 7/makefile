FLAGS = -std=c++98 -g -Wall -Werror

main: StringLib.o main.o
	g++ $(FLAGS) StringLib.o main.o -o main

StringLib.o: StringLib.cpp StringLib.h
	g++ $(FLAGS) -c StringLib.cpp -o StringLib.o

main.o: main.cpp
	g++ $(FLAGS) -c main.cpp -o main.o

run: main
	./main
