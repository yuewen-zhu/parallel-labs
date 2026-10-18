#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int myrank, size;

    //初始化 MPI 环境
    MPI_Init(&argc, &argv);

    // 获取当前进程在 MPI_COMM_WORLD 中的编号
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // 获取 MPI_COMM_WORLD 中的进程总数
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Hello world from process %d of %d\n", myrank, size);

    // 结束 MPI 环境
    MPI_Finalize();

    return 0;
}