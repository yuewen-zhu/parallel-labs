//故意的错误示例
#include <cstdio>
#include <thread>

int main()
{
    const int repetitions = 1000000;
    int count = 0;

    auto work = [&] {
        for(int i = 0; i < repetitions; ++i) {
            ++count;
        }
    };

    std::thread t1(work);
    std::thread t2(work);

    t1.join();
    t2.join();

    std::printf("expected=%d\n", 2 * repetitions);
    std::printf("actual=%d\n", count);

    return 0;

}