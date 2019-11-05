CC=g++

all: prog

test: prog
	./prog

prog: main.o BigInt.o
	$(CC) -o prog main.o BigInt.o

main.o: main.cpp BigInt.h
	$(CC) -O2 -c main.cpp

BigInt.o: BigInt.cpp BigInt.h
	$(CC) -O2 -c BigInt.cpp

clean:
	rm -rf *.o prog

