CC=g++

all: prog

test: prog
	./prog

prog: main.o
	$(CC) -o prog main.o

main.o: main.cpp vector.h
	$(CC) -O2 -c main.cpp

clean:
	rm -rf *.o prog

