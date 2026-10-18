# mini-rwlock: 手搓写优先读写锁

纯 C++17 手写实现的写优先读写锁，不依赖 `std::shared_mutex`，仅使用 `std::mutex` 和 `std::condition_variable` 构建。

## 核心规则
- **读共享**：多个读者可同时持有读锁。
- **写独占**：写者持有写锁时，其他读者和写者必须等待。
- **写优先**：一旦有写者排队（`waiting_writers > 0`），新来的读者必须等待，防止写者饥饿。

## 验证
包含三个测试：
1. `test_rwlock`：并发写，校验共享计数无数据竞争。
2. `test_read_write`：读写并发，校验读锁内数据一致性。
3. `test_writer_first`：卡时序测试，证明写者排队时新读者无法插队。