#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc < 3 || argc > 4) {
        std::puts("Usage: ./matrix rows workers [width]");
        return 1;
    }   

    const int rows = std::atoi(argv[1]);  // 输出矩阵行数。
    const int p = std::atoi(argv[2]);     // 工作线程数。
    const int n = argc == 4 ? std::atoi(argv[3]) : 2;

    if (rows < 1 || rows > 32768 || p < 0 || p > 64
        || n < 1 || n > 1024) {
        std::puts("Invalid size or worker count");
        return 1;
    }

    std::vector<int> a(rows * n), b(n * n);
    std::vector<int> c(rows * n), expected(rows * n);

    for (int i = 0; i < rows * n; ++i) {
        a[i] = i % 7 + 1;
    }
    for (int i = 0; i < n * n; ++i) {
        b[i] = i % 5 + 5;
    }

    auto multiply = [&](int begin, int end, std::vector<int>& out) {
        for (int i = begin; i < end; ++i) {
            for (int j = 0; j < n; ++j) {
                int sum = 0;
                for (int k = 0; k < n; ++k) {
                    sum += a[i * n + k] * b[k * n + j];
                }
                out[i * n + j] = sum;
            }
        }
    };

    // 先计算串行参考结果，不计入下面的计时。
    multiply(0, rows, expected);

    std::printf("rows=%d width=%d workers=%d\n", rows, n, p);
    for (int t = 0; t < p; ++t) {
        int begin = rows * t / p;
        int end = rows * (t + 1) / p;
        std::printf("T%d: rows [%d, %d)\n", t, begin, end);
    }

    std::vector<std::thread> workers;
    workers.reserve(p);

    auto start = std::chrono::steady_clock::now();   

    if (p == 0) {
        // 为下一步保留的串行模式：由主线程计算。
        multiply(0, rows, c);
    } else {
        for(int t = 0; t < p; ++t) {
            int begin = rows * t / p;
            int end = rows * (t + 1) / p;

            workers.emplace_back([&, begin, end] {
                multiply(begin, end, c);
            });
        }

        for (auto& worker : workers) {
            worker.join();
        }       
    } 

    auto finish = std::chrono::steady_clock::now();
    double elapsed =
        std::chrono::duration<double, std::milli>(finish - start).count();

    if (rows <= 7 && n == 2) {
        for (int i = 0; i < rows; ++i) {
            std::printf("%d %d\n", c[i * n], c[i * n + 1]);
        }
    }

    bool ok = c == expected;  // 逐元素比较整个结果。
    std::printf("check=%s\n", ok ? "PASS" : "FAIL");
    std::printf("elapsed_ms=%.3f\n", elapsed);
    return ok ? 0 : 1;    
}