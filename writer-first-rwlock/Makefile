CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pthread

all: test_rwlock test_read_write test_writer_first

test_rwlock: rwlock.cpp test_rwlock.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_read_write: rwlock.cpp test_read_write.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test_writer_first: rwlock.cpp test_writer_first.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f test_rwlock test_read_write test_writer_first

run:
	@echo "--- 1. 共享计数保护 ---"
	@./test_rwlock
	@echo "\n--- 2. 读写互斥验证 ---"
	@./test_read_write
	@echo "\n--- 3. 写优先防饥饿验证 ---"
	@./test_writer_first