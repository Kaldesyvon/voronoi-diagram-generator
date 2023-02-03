CC=g++
CFLAGS=-std=c++14 -lm# -Wall -Werror
OUTPUT=voronoi

all: 
	$(CC) main.cpp $(CFLAGS) -o $(OUTPUT)
