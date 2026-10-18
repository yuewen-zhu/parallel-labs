#include <cstdio>
#include <mutex>
#include <thread> 
#include <barrier>

int main()
{

    std::mutex a, b;

    std::barrier ready(2);

    std::thread t1([&] {
        a.lock();   // t1 申请持有锁 A
        std::puts("T1: got A");
        
        ready.arrive_and_wait();

        std::puts("T1: waiting for B");

        b.lock(); // t1 想申请锁 B, 但是此时 B 锁在 t2 手里

        std::puts("T1:got A and B");

        b.unlock();
        a.unlock();

    });
    // T2 的执行进度可以和 T1 交错，不能理解成“等 T1 完成后才执行 T2”。
    std::thread t2([&] {
        b.lock();  
        std::puts("T1: got B");
        
        ready.arrive_and_wait();

        std::puts("T2: waiting for B");

        a.lock(); 

        std::puts("T2:got A and B");

        a.unlock();
        b.unlock();

    }); //结束 T2 的匿名函数及线程对象的构造, 主线程继续向下执行
    
    t1.join(); // join() 的作用是等待线程结束
    t2.join();

    std::puts("Finished");
}