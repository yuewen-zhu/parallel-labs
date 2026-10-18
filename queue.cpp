#include <cstdio>
#include <cstdlib>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

int main(int argc, char* argv[]) 
{
    const int n = argc > 1 ? std::atoi(argv[1]) : 5;
    if(n < 0 || n > 5) {
        std::puts("Please use 0...5 tasks");
        return 1;
    }

    std::queue<int> tasks;
    std::mutex gate;
    std::condition_variable cv;

    bool done = false;
    int count = 0;
    int total = 0;
    int senn[6] = {};

    std::thread worker([&] {
        while (true) {
            std::unique_lock<std::mutex> lock(gate);

            cv.wait(lock, [&] {
                return done || !tasks.empty();
            });

            if (done && tasks.empty()) {
                break;
            }

            int task = tasks.front();  
            tasks.pop();             
            lock.unlock();            

            total += task;
            ++count;
            ++seen[task];
        }
    });

    for(int i = 1; i <= n; ++i) {
        gate.lock();
        tasks.push(i);
        gate.unlock();

        cv.notify_one();
    }

    gate.lock();
    done = true;
    gate.unlock();
    cv.notify_one();

    worker.join();

    bool ok = count == n && total == n * (n + 1) / 2;
    for (int i = 1; i <= n; ++i) {
        ok = ok && seen[i] == 1;  // 每个任务恰好执行一次。
    }

    std::printf("count=%d\n", count);
    std::printf("total=%d\n", total);
    std::printf("check=%s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;    
}