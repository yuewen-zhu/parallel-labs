#include <cstdio>
#include <thread>
#include <mutex> //互斥锁

int main()
{
    const int repetitions = 1000000;
    int count = 0;

    std::mutex gate;  // 两个线程共用同一把锁。

    auto work = [&] {
        for(int i = 0; i < repetitions; ++i) {
            gate.lock();    // 申请锁，拿不到就等待
            ++count;        // 持锁期间完成这次递增
            gate.unlock();  // 释放锁
        }
    };

    std::thread t1(work);
    std::thread t2(work);

    t1.join();
    t2.join();

    int expected = 2 * repetitions;

    std::printf("expected=%d\n", expected);
    std::printf("actual=%d\n", count);

    bool ok = count == expected;
    std::printf("check=%s\n", ok ? "PASS" : "FAIL");

    return ok ? 0 : 1;

}