#include "Parallel.h"

#include "../../app/src/Render.h"

/* Local Definitions */
static std::vector<std::thread> threads;
static bool shutdownThreads = false;

extern thread_local int tl_threadIndex;

static parallelForLoop *workList = nullptr;
static std::mutex workListMutex;
static std::condition_variable workListCondition;


static void workerThreadFunc(int tIndex) {
    tl_threadIndex = tIndex;
    std::unique_lock<std::mutex> lock(workListMutex);
    while(!shutdownThreads){

        // Sleep until there are more tasks
        if (!workList){
            workListCondition.wait(lock);
        }
        // Get work from workList and run iterations
        else {
            parallelForLoop &loop = *workList;

            // Find the set of loop iterations to run next
            int64_t indexStart = loop.nextIndex;
            int64_t indexEnd = std::min(indexStart + loop.chunkSize, loop.maxIndex);

            // Update loop to iteration that this thread will run
            loop.nextIndex = indexEnd;
            if (loop.nextIndex == loop.maxIndex) {
                workList = loop.next;
            }
            loop.activeWorkers++;

            lock.unlock();
            for (int index = indexStart; index < indexEnd; ++index) {
                // If function in the case that there is another type of loop
                if (loop.func1D) {
                    loop.func1D(index);
                }
                lock.lock();
            }
            if (loop.Finished())
                workListCondition.notify_all();
        }
    }
    //ReportThreadStatus();
}


void parallelFor(std::function<void(int)> func, int64_t count, int chunkSize)
{
    Renderer renderer;
    if (renderer.GetSettings().threadCount == 1 || count < chunkSize) {
        for(int i = 0; i < count; ++i)
            func(i);
        return;
    }

    if (threads.size() == 0 ) {
        tl_threadIndex = 0;
        for (int i = 0; i < numSystemCores() - 1; ++i){
            threads.push_back(std::thread(workerThreadFunc, i + 1));
        }
    }

    parallelForLoop loop(func, count, chunkSize);

    workListMutex.lock();
    loop.next = workList;
    workList = &loop;
    workListMutex.unlock();

    // Notify worker threads of work
    std::unique_lock<std::mutex> lock(workListMutex);
    workListCondition.notify_all();

    while (!loop.Finished())
    {
        int64_t indexStart = loop.nextIndex;
        int64_t indexEnd = std::min(indexStart + loop.chunkSize, loop.maxIndex);

        loop.nextIndex = indexEnd;
        if (loop.nextIndex == loop.maxIndex){
            workList = loop.next;
        }
        loop.activeWorkers++;

        lock.unlock();
        for (int index = indexStart; index < indexEnd; ++index) {
            if (loop.func1D) {
                loop.func1D(index);
            }
        }
        lock.lock();

        loop.activeWorkers--;
    }
}