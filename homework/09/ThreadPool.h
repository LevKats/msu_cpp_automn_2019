#pragma once
#include <vector>
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

