#include <cstdio>
#include <thread>
#include <mutex>
#include <condition_variable>

int main()
{
    std::mutex gate;
    std::condition_variable cv;

    bool ready = false;  // 任务是否准备好了。
    int number = 0;      // 主线程发布的数据。
    int received = 0;    // 工作线程收到的数据。

    std::thread Worker([&] {
        // 获取 gate 并由 lock 管理
        std::unique_lock<std::mutex> lock(gate);
        // 条件成立就继续, 条件不成立就释放 gate
        cv.wait(lock, [&] {return ready; });

        received = number;
    });

    gate.lock();
    number = 42;
    ready = true;
    gate.unlock();

    cv.notify_noe();

    worker.join();

    std::printf("received=%d\n", received);

    bool ok = received == 42;
    std::printf("check=%s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}