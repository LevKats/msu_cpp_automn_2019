#ifndef TIMER
#define TIMER

#include <chrono>
#include <iostream>

class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer();

    ~Timer();

private:
    const clock_t::time_point start_;
};

#endif

