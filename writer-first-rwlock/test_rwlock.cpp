#include "rwlock.hpp"
#include <cstdio>
#include <thread>

int main()
{
    RWLock rw;
    int counter = 0;

    auto add = [&] {
        for (int i = 0; i < 100000; ++i) {
            rw.lock_write();
            ++counter;
            rw.unlock_write();
        }
    };

    std::thread t1(add);
    std::thread t2(add);

    t1.join();
    t2.join();

    bool ok = counter == 200000;

    std::printf("counter=%d expected=200000\n", counter);
    std::printf("check=%s\n", ok ? "PASS" : "FAIL");

    return ok ? 0 : 1;
}