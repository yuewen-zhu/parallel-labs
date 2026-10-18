#pragma once

#include <mutex>
#include <condition_variable>

struct RWLock {
    int active_readers = 0;
    bool writer_active = false;
    int waiting_writers = 0;

    std::mutex gate;
    std::condition_variable read_ready;
    std::condition_variable write_ready;

    void lock_read();
    void unlock_read();
    void lock_write();
    void unlock_write();
};