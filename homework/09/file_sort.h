#pragma once
#include <vector>
#include "ThreadPool.h"
#include <fstream>
#include <string>
#include <exception>
#include <algorithm>
#include <queue>
#include <iostream>
#include <cstdio>

const int chunk_size = 10 * 1000;

class Sorter_One_Thread {
public:
    Sorter_One_Thread(std::vector<std::string> chunks, int counter);
    std::string sort();
    ~Sorter_One_Thread();
private:
    std::vector<std::string> chunks;
    std::vector<std::string> files;
    int counter;
};

std::vector<std::string> make_chunks(std::string filename);

void merge(std::pair<std::string, std::string> p, std::string name);

void sort_file(std::string name, std::string new_name);

void sort(std::string filename, std::string to);

