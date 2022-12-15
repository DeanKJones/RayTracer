#ifndef RAYTRACING_PARALLEL_H
#define RAYTRACING_PARALLEL_H

#include <iostream>
#include "thread"

class parallelForLoop
{
public:
    parallelForLoop(std::function<void(int)> func1D, int64_t maxIndex, int chunkSize) :
            func1D(std::move(func1D)), maxIndex(maxIndex),
            chunkSize(chunkSize) { }

public:
    std::function<void(int)> func1D;

    const int64_t maxIndex;
    const int chunkSize;

    int64_t nextIndex = 0;
    int activeWorkers = 0;

    parallelForLoop *next = nullptr;

    bool Finished() const { return nextIndex >= maxIndex && activeWorkers == 0; }
};

int numSystemCores() { return std::thread::hardware_concurrency(); }

#endif //RAYTRACING_PARALLEL_H
