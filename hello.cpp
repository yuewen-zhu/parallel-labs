#include <cstdio>
#include <pthread.h>

// pthread 要求工作函数接收一个 void*，返回一个void*
// 不用输入参数, 省略参数名
void* work(void*)
{
    std::puts("Worker: hello");

    return nullptr;
}

int main()
{
    pthread_t t;

    std::puts("Main: before create");

    int err = pthread_create(&t, nullptr, work, nullptr);

    if(err != 0) {
        std::printf("pthread_create failed: %d\n", err);
        return 1;
    }

    err = pthread_join(t, nullptr);

    if(err != 0) {
        std::printf("pthread_join failed: %d\n", err);
        return 1;
    }

    std::puts("Main: after join");
    return 0;
}