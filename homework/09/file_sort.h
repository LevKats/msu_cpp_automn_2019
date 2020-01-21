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
    Sorter_One_Thread(const std::vector<std::string> &chunks, int counter);
    std::string sort();
    ~Sorter_One_Thread();
private:
    std::vector<std::string> chunks;
    std::vector<std::string> files;
    int counter;
};

std::vector<std::string> make_chunks(const std::string &filename);

void merge(const std::pair<std::string, std::string> &p, const std::string &name);

void sort_file(const std::string &name, const std::string &new_name);

void sort(const std::string &filename, const std::string &to);

