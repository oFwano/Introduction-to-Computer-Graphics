CC = g++

all: main

main: main.o FruitTetris.o
	$(CC) -o FruitTetris main.o FruitTetris.o -lopengl32 -lglew32 -lfreeglut -lglu32

main.o: main.cpp FruitTetris.h
	$(CC) -c main.cpp

FruitTetris.o: FruitTetris.cpp FruitTetris.h
	$(CC) -c FruitTetris.cpp

run:
	FruitTetris
