#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <mutex>
#include <queue>
#include <thread>

int main(int argc, char* argv[])
{
    // 0：固定分工；1：动态领取。    
    const int mode = argc > 1 ? std::atoi(argv[1]) : 0;
    if(mode != 0 && mode != 1) {
        std::puts("Please use mode 0 or 1");
        return 1;
    }

    const int cost[4] = {400, 400, 100, 100};

    std::queue<int> tasks;
    for (int i = 0; i < 4; ++i) {
        tasks.push(i);  // 创建线程前，先准备好全部任务。
    }    

    std::mutex gate;
    int count[2] = {};        // 每个线程处理的任务数。
    int assigned_ms[2] = {};  // 每个线程领取任务的标称耗时之和。
    int seen[4] = {};         // 每个任务执行了几次。

    auto process = [&](int id, int task) {
        // 根据任务编号, 确定耗时
        std::this_thread::sleep_for(
            std::chrono::milliseconds(cost[task])
        );

        ++count[id];
        assigned_ms[id] += cost[task];
        ++seen[task];
    };

    auto work = [&](int id) {
        if (mode == 0) {
            for (int task = id * 2; task < (id + 1) * 2; ++task) {
                process(id, task);
            }
        } else {
            while (true) {
                std::unique_lock<std::mutex> lock(gate);

                if (tasks.empty()) {
                    break;
                }
                
                int task = tasks.front();
                tasks.pop();
                lock.unlock();

                process(id, task);
            }
        }
    };

    auto start = std::chrono::steady_clock::now();

    std::thread t1(work, 0);
    std::thread t2(work, 1);
    t1.join();
    t2.join();   

    auto end = std::chrono::steady_clock::now();

    double elapsed = std::chrono::duration<double, std::milli>(end - start).count();

    bool ok = count[0] + count[1] == 4;
    ok = ok && assigned_ms[0] + assigned_ms[1] == 1000;

    for (int i = 0; i < 4; ++i) {
        ok = ok && seen[i] == 1;
    }

    std::printf("mode=%s\n", mode == 0 ? "static" : "dynamic");

    for (int id = 0; id < 2; ++id) {
        std::printf("T%d: tasks=%d assigned_ms=%d\n",
                    id, count[id], assigned_ms[id]);
    }

    std::printf("elapsed_ms=%.1f\n", elapsed);
    std::printf("check=%s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}
