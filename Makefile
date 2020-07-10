CC = g++

FLAGS = -std=c++20 -Ofast -march=native

EXE = swap

swap: main.cc
	$(CC) $(FLAGS) main.cc -o $(EXE)

clean:
	rm swap