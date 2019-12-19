prog: main.cpp file_sort.h
	g++ -o prog main.cpp -O2 -std=c++14 -lpthread

test: prog
	./prog

clean:
	rm -rf *.o ./prog *.txt
