CC=g++

all: prog

test: prog
	./prog

prog: main.o linear_allocator.o
	$(CC) -o prog main.o linear_allocator.o

main.o: main.cpp linear_allocator.h
	$(CC) -O2 -c main.cpp

linear_allocator.o: linear_allocator.cpp linear_allocator.h
	$(CC) -O2 -c linear_allocator.cpp

clean:
	rm -rf *.o prog

