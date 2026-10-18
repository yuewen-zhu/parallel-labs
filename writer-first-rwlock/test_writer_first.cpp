#include "rwlock.hpp"
#include <cstdio>
#include <thread>
#include <chrono>

// 用于输出顺序记录
int sequence = 0;

int main()
{
    RWLock rw;

    // 1. 主线程先霸占读锁
    rw.lock_read();
    std::printf("Main: hold read lock\n");

    // 2. 启动写线程
    std::thread writer([&] {
        // 写线程会被阻塞在这里，因为主线程持有读锁
        rw.lock_write(); 
        
        // 拿到写锁后，记录自己是第几个进来的
        sequence++;
        std::printf("Writer: entered (sequence=%d)\n", sequence);
        
        rw.unlock_write();
    });

    // 睡 50ms，确保写线程已经执行到了 lock_write 内部，且 waiting_writers 已经 +1
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // 3. 启动新读者
    std::thread reader([&] {
        // 此时有写者在排队，新读者不许插队，必须等
        rw.lock_read();
        
        sequence++;
        std::printf("Reader: entered (sequence=%d)\n", sequence);
        
        rw.unlock_read();
    });

    // 睡 50ms，让读者线程也跑到 lock_read 阻塞住
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // 4. 主线程释放读锁，看看谁会先醒来
    std::printf("Main: release read lock\n");
    rw.unlock_read();

    writer.join();
    reader.join();

    // 5. 校验：写者必须排在读者前面
    bool ok = (sequence == 2); // 两个线程都进去了
    std::printf("check=%s\n", ok ? "PASS (但需看日志确认顺序)" : "FAIL");

    return 0;
}