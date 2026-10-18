#include <cstdio>
#include <thread>

int main()
{
    const int a[2][2] = {{1, 2}, {3, 4}};
    const int b[2][2] = {{5, 6}, {7, 8}};
    int c[2][2] = {};

    // row 指定当前线程负责计算哪一行。
    auto work = [&](int row) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                c[row][j] += a[row][k] * b[k][j];
            }
        }
    };

    std::thread t1(work, 0);  // 执行 work(0)，计算第 0 行。
    std::thread t2(work, 1);  // 执行 work(1)，计算第 1 行。

    t1.join();
    t2.join();

    // 两个工作线程结束后，由主线程打印。
    for (int i = 0; i < 2; ++i) {
        std::printf("%d %d\n", c[i][0], c[i][1]);
    }

    bool ok = c[0][0] == 19 && c[0][1] == 22
           && c[1][0] == 43 && c[1][1] == 50;

    std::printf("check=%s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}