#include "timer.h"

int main() {
	const size_t HSIZE = 10 * 1000;
	const size_t WSIZE = 10 * 1000;
	unsigned int **mas = new unsigned int*[HSIZE];
	for (int i = 0; i < HSIZE; ++i)
		mas[i] = new unsigned int[WSIZE];
	unsigned int sum = 0;
	Timer t;
	for (int j = 0; j < WSIZE; ++j)	
		for (int i = 0; i < HSIZE; ++i) {
			volatile unsigned int a = mas[i][j];
			sum += a;
		}
	return 0;
}

