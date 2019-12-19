#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <exception>
#include <algorithm>
#include <pthread.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <functional>
#include <atomic>
#include <iostream>

class ThreadPool
{
public:
    using Function = std::function<void()>;
    explicit ThreadPool(size_t poolSize) {
	destroyed.store(false);
        while (--poolSize) {
    	    threads.emplace_back([this](){
	        while (!destroyed.load()) {
		    std::unique_lock<std::mutex> q_lock(queue_lock);
		    if (!tasks.empty()) {
		        auto func = tasks.front();
		        tasks.pop();
		        q_lock.unlock();
		        func();
		    }
                    else {
		        wake_up.wait(q_lock, [this](){ return destroyed.load() ||
		        !tasks.empty(); });
		    }
	        }
	     });
        }
    }

    ~ThreadPool() {
        destroyed.store(true);
        wake_up.notify_all();
        for (auto &thread_ : threads)
            thread_.join();
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
        auto promise = std::make_shared< std::promise<decltype(func(args...))> >();
        std::future<decltype(func(args...))> future = promise->get_future();

        auto task = [](std::shared_ptr< std::promise<decltype(func(args...))> > p,
                       Func func, Args... args) {
            ThreadPool::wrapper(p, func, args...);
        };
        {
            std::lock_guard<std::mutex> guard(queue_lock);
            tasks.push(std::bind(task, promise, func, args...));
        }
        wake_up.notify_one();
        return future;
    }
private:
    std::vector<std::thread> threads;
    std::queue<Function> tasks;
    std::atomic<bool> destroyed;
    std::condition_variable wake_up;
    std::mutex queue_lock;

    template <class Func, class... Args>
    static void wrapper(std::shared_ptr< std::promise<void> > p,
                       Func func, Args... args) {
        func(args...);
        p->set_value();
    }
    template <class Pointer, class Func, class... Args>
    static void wrapper (Pointer p, Func func, Args... args) {
        p->set_value(func(args...));
    };
};


const int chunk_size = 10 * 1000;

std::vector<std::string> make_chunks(std::string filename) {
    std::ifstream is (filename, std::ifstream::binary);
    std::vector<std::string> res;
    if (is) {
        // get length of file:
        is.seekg (0, is.end);
        size_t length = is.tellg() / sizeof(uint64_t);
        is.seekg (0, is.beg);
        std::vector<uint64_t> myVec;
        int i = 0;
        
        while (!(length == 0)) {
            size_t size = std::min(length, chunk_size * sizeof(uint64_t));
            myVec.resize(size);
            is.read(reinterpret_cast<char*>(&myVec.front()),
                    myVec.size()*sizeof(uint64_t));
            std::string chunk_filename = std::to_string(i++) +
                    std::string("chunk.txt");
            res.push_back(chunk_filename);
            std::ofstream f(chunk_filename, std::ios::binary);
            f.write(reinterpret_cast<char*>(myVec.data()),
                    myVec.size() * sizeof(uint64_t));
            f.close();
            length -= size;
        }
        
        is.close();
        return res;
    }
    else{
        throw std::runtime_error("file didn't open");
    }
}


int merge(std::pair<std::string, std::string> p, std::string name) {
    std::ifstream is1 (p.first, std::ifstream::binary);
    std::ifstream is2 (p.second, std::ifstream::binary);
    std::ofstream os(name, std::ios::binary);
    is1.seekg (0, is1.end);
    size_t length1 = is1.tellg() / sizeof(uint64_t);
    is1.seekg (0, is1.beg);
    is2.seekg (0, is2.end);
    size_t length2 = is2.tellg() / sizeof(uint64_t);
    is2.seekg (0, is2.beg);
    
    uint64_t a;  
    is1.read(reinterpret_cast<char*>(&a), sizeof(uint64_t));
    uint64_t b;  
    is2.read(reinterpret_cast<char*>(&b), sizeof(uint64_t));
    length1--;
    length2--;
    
    while (length1 && length2) {
        if (a < b) {
            length1--;
            os.write(reinterpret_cast<char*>(&a), sizeof(uint64_t));
            is1.read(reinterpret_cast<char*>(&a), sizeof(uint64_t));
        }
        else {
            length2--;
            os.write(reinterpret_cast<char*>(&b), sizeof(uint64_t));
            is2.read(reinterpret_cast<char*>(&b), sizeof(uint64_t));
        }
    }
    while (length1) {
        length1--;
        os.write(reinterpret_cast<char*>(&a), sizeof(uint64_t));
        is1.read(reinterpret_cast<char*>(&a), sizeof(uint64_t));
    }
    while (length2) {
        length2--;
        os.write(reinterpret_cast<char*>(&b), sizeof(uint64_t));
        is2.read(reinterpret_cast<char*>(&b), sizeof(uint64_t));
    }
    is1.close();
    is2.close();
    os.close();
    return 0;
}

int sort_file(std::string name, std::string new_name) {
    std::ifstream is (name, std::ifstream::binary);
    std::ofstream os(new_name, std::ios::binary);
    is.seekg (0, is.end);
    size_t length = is.tellg() / sizeof(uint64_t);
    is.seekg (0, is.beg);
    
    std::vector<uint64_t> myVec(length);
    is.read(reinterpret_cast<char*>(&myVec.front()),
                    myVec.size()*sizeof(uint64_t));
    std::sort(myVec.begin(), myVec.end());
    os.write(reinterpret_cast<char*>(myVec.data()),
                    myVec.size() * sizeof(uint64_t));
    is.close();
    os.close();
    return 0;
}

std::string sort_1_thread(std::vector<std::string> chunks, int counter) {
    std::queue<std::string> names;
    for (auto name: chunks) {
        auto new_name = std::to_string(counter++) + "file.txt";
        sort_file(name, new_name);
        names.push(new_name);
    }
    while (!(names.size() == 1)) {
        auto new_name = std::to_string(counter++) + "file.txt";
        auto name1 = names.front();
        names.pop();
        auto name2 = names.front();
        names.pop();
        merge(std::make_pair(name1, name2), new_name);
        std::cout << "merge " << name1 << " " << name2 << "->" << new_name << std::endl; 
        names.push(new_name);
    }
    return names.front();
}

void sort(std::string filename, std::string to) {
    auto chunks = make_chunks(filename);
    std::vector<std::string> chunks1(chunks.size() / 2),
    chunks2(chunks.size() - chunks.size() / 2);
    std::copy(chunks.begin(), chunks.begin() + chunks1.size(), chunks1.begin());
    std::copy(chunks.begin() + chunks1.size(), chunks.end(), chunks2.begin());
    ThreadPool pool(2);
    auto p1 = pool.exec(sort_1_thread, chunks1, 0);
    auto p2 = pool.exec(sort_1_thread, chunks2, 10 * chunks1.size());
    merge(std::make_pair(p1.get(), p2.get()), to);
}

