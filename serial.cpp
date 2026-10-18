#include <cstdio>

int main()
{
    const int a[2][2] = {{1, 2}, {3, 4}};
    const int b[2][2] = {{5, 6}, {7, 8}};

    int c[2][2] = {};  // 所有元素初始化为 0。

    for (int i = 0; i < 2; ++i) {          // 输出的行。
        for (int j = 0; j < 2; ++j) {      // 输出的列。
            for (int k = 0; k < 2; ++k) {  // 对应元素相乘，再累加。
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    for (int i = 0; i < 2; ++i) {
        std::printf("%d %d\n", c[i][0], c[i][1]);
    }

    bool ok = c[0][0] == 19 && c[0][1] == 22
           && c[1][0] == 43 && c[1][1] == 50;

    std::printf("check=%s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}