#include "rwlock.hpp"
#include <cstdio>
#include <thread>

int main()
{
    RWLock rw;
    int counter = 0;
    int twice = 0;
    int errors[2] = {};

    auto write = [&] {
        for (int i = 0; i < 100000; ++i) {
            rw.lock_write();
            ++counter;
            twice = 2 * counter;
            rw.unlock_write();
        }
    };

    auto read = [&](int id) {
        for (int i = 0; i < 100000; ++i) {
            rw.lock_read();
            if (twice != 2 * counter) {
                ++errors[id];
            }
            rw.unlock_read();
        }
    };

    std::thread r1(read, 0);
    std::thread r2(read, 1);
    std::thread w1(write);
    std::thread w2(write);

    r1.join();
    r2.join();
    w1.join();
    w2.join();

    int total_errors = errors[0] + errors[1];
    bool ok = counter == 200000 && twice == 400000 && total_errors == 0;

    std::printf("counter=%d twice=%d read_errors=%d\n", counter, twice, total_errors);
    std::printf("check=%s\n", ok ? "PASS" : "FAIL");

    return ok ? 0 : 1;
}