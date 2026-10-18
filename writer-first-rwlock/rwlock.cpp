#include "rwlock.hpp"

void RWLock::lock_read()
{
    std::unique_lock<std::mutex> guard(gate);

    read_ready.wait(guard, [&] {
        return !writer_active && waiting_writers == 0;
    });

    ++active_readers;
}

void RWLock::unlock_read()
{
    std::unique_lock<std::mutex> guard(gate);

    --active_readers;

    if (active_readers == 0 && waiting_writers > 0) {
        write_ready.notify_one();
    }
}

void RWLock::lock_write()
{
    std::unique_lock<std::mutex> guard(gate);

    ++waiting_writers;

    write_ready.wait(guard, [&] {
        return !writer_active && active_readers == 0;
    });

    --waiting_writers;
    writer_active = true;
}

void RWLock::unlock_write()
{
    std::unique_lock<std::mutex> guard(gate);

    writer_active = false;

    if (waiting_writers > 0) {
        write_ready.notify_one();
    } else {
        read_ready.notify_all();
    }
}