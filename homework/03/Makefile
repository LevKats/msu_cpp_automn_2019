CC=g++

all: prog

test: prog
	./prog

prog: main.o Matrix.o
	$(CC) -o prog main.o Matrix.o

main.o: main.cpp Matrix.h
	$(CC) -O2 -c main.cpp

Matrix.o: Matrix.cpp Matrix.h
	$(CC) -O2 -c Matrix.cpp

clean:
	rm -rf *.o prog

