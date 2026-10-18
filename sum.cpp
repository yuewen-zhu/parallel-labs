#include <cstdio>
#include <thread>

int main()
{
    int left = 0;
    int right = 0;

    std::thread t1([&] {
        for(int i = 1; i <= 5; ++i) {
            left += i;
        }
    });
    std::thread t2([&] {
        for(int i = 6; i <= 10; ++i) {
            right += i;
        }
    });

    t1.join();
    t2.join();

    int total = left + right;

    std::printf("left=%d\n", left);
    std::printf("right=%d\n", right);
    std::printf("total=%d\n", total);

    bool ok = left == 15 && right == 40 && total == 55;
    std::printf("check=%s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;    
}