#include <cstdio>
#include <mutex>
#include <thread>

int main()
{
    std::mutex a, b;

    auto work = [&](const char* name) {
        a.lock();  // 都先申请 A。
        std::printf("%s: got A\n", name);

        b.lock();  // 拿到 A 后，再申请 B。
        std::printf("%s: got A and B\n", name);

        b.unlock();
        a.unlock();
    };

    std::thread t1(work, "T1");
    std::thread t2(work, "T2");

    t1.join();
    t2.join();

    std::puts("Finished");
    return 0;
}