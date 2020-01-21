prog: main.cpp file_sort.h file_sort.cpp ThreadPool.h
	g++ -o prog main.cpp file_sort.cpp -O2 -std=c++14 -lpthread

test: prog
	python3 generator.py
	./prog
	python3 checker.py

clean:
	rm -rf *.o ./prog *.txt
