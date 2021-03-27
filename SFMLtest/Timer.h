#pragma once

#include <chrono>

class Timer {
    std::chrono::high_resolution_clock::time_point start;
public:
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<double>(end - start).count();
        std::printf("Time: %9.8f s\tPer second: %4.2f\n", time, 1/time);
    }
};