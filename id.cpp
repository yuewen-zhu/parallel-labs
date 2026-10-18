#include <cstdio>
#include <thread>
#include <unistd.h>
#include <sys/syscall.h>

void show(const char* name)
{
    long pid = getpid();
    long tid = syscall(SYS_gettid);

    std::printf("%s: PID=%ld TID=%ld\n", name, pid, tid);
}

int main()
{
    show("Main");
    std::thread t1(show, "T1");
    std::thread t2(show, "T2");

    t1.join();
    t2.join();

    return 0;
}