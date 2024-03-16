CC=g++
FLAGS=-std=c++20 -pedantic -Wall -Wextra -Wno-unused-variable
OUTPUT=lab01.out

all: main.cpp sort.o pair.o
	$(CC) $(FLAGS) main.cpp sort.o pair.o -o $(OUTPUT)
sort.o: sort.cpp sort.h
	$(CC) $(FLAGS) -c sort.cpp
pair.o: pair.cpp pair.h
	$(CC) $(FLAGS) -c pair.cpp
benchmark: sort.o pair.o benchmark.cpp
	$(CC) $(FLAGS) sort.o pair.o benchmark.cpp -o benchmark.out
clean:
	rm *.o $(OUTPUT)
