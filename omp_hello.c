#include <stdio.h>
#include <omp.h>

int main(void)
{
    #pragma omp parallel
    {
        // 获取当前线程的编号,从 0 开始
        int id = omp_get_thread_num();

        //获取当前线程团队的线程总数
        int n = omp_get_num_threads();

        printf("Hello from thread %d of %d\n", id, n);
    }

    return 0;
}