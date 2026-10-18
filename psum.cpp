#include <cstdio>
#include <pthread.h>

struct Task {
    int begin;
    int end;
    int result;  
};

void* work(void* arg)
{
    Task* task = static_cast<Task*>(arg);

    for(int i = task->begin; i <= task->end; ++i) {
        task->result += i;
    }

    return nullptr;
}

int main()
{
    Task left{1, 5, 0};
    Task right{6, 10, 0};

    pthread_t t1, t2;

    int err = pthread_create(&t1, nullptr, work, &left);
    if(err != 0) {
        std::printf("create T1 failed: %d\n", err);
        return 1;
    }

    err = pthread_create(&t2, nullptr, work, &right);
    if(err != 0) {
        pthread_join(t1, nullptr);
        std::printf("create T2 failed: %d\n", err);
        return 1;
    }

    int err1 = pthread_join(t1, nullptr);
    int err2 = pthread_join(t2, nullptr);

    if(err1 != 0 || err2 != 0) {
        std::printf("pthread_join failed: %d %d\n", err1, err2);
        return 1;
    }

        int total = left.result + right.result;

    std::printf("left=%d\n", left.result);
    std::printf("right=%d\n", right.result);
    std::printf("total=%d\n", total);

    bool ok = left.result == 15 && right.result == 40 && total == 55;
    std::printf("check=%s\n", ok ? "PASS" : "FAIL");

    return ok ? 0 : 1;

}