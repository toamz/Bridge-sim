#pragma once

#include <chrono>

static double avgSum = 0;
static size_t avgLen = 0;

class Timer {
    std::chrono::high_resolution_clock::time_point start;
public:
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<double>(end - start).count();
        avgSum += time;
        ++avgLen;
        std::printf("Time: %9.8f s\tPer second: %4.2f\tAvg: %9.8f\n", time, 1/time, avgSum/avgLen);
    }
};