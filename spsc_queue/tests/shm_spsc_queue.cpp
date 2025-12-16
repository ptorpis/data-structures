#include "spsc_queue_shm.hpp"
#include <chrono>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

// Helper to create shared memory for tests
class ShmHelper {
public:
    ShmHelper(const char* name, size_t size)
        : name_(name), size_(size), ptr_(nullptr), fd_(-1) {

        shm_unlink(name_);

        fd_ = shm_open(name_, O_CREAT | O_RDWR, 0666);
        if (fd_ == -1) {
            throw std::runtime_error("shm_open failed");
        }

        if (ftruncate(fd_, size_) == -1) {
            close(fd_);
            throw std::runtime_error("ftruncate failed");
        }

        ptr_ = mmap(nullptr, size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
        if (ptr_ == MAP_FAILED) {
            close(fd_);
            throw std::runtime_error("mmap failed");
        }
    }

    ~ShmHelper() {
        if (ptr_) munmap(ptr_, size_);
        if (fd_ != -1) close(fd_);
        shm_unlink(name_);
    }

    void* get() { return ptr_; }

    // For child process to open existing shared memory
    static void* open(const char* name, size_t size) {
        int fd = shm_open(name, O_RDWR, 0666);
        if (fd == -1) return nullptr;

        void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        return ptr == MAP_FAILED ? nullptr : ptr;
    }

private:
    const char* name_;
    size_t size_;
    void* ptr_;
    int fd_;
};

template <typename T> size_t calculate_queue_size(size_t capacity) {
    size_t buffer_size = std::bit_ceil(capacity + 1);
    return sizeof(ptorpis::spsc_queue_shm<T>) + sizeof(T) * buffer_size;
}

// Single Process Tests

TEST(SPSCQueueShm, BasicPushPop) {
    const size_t capacity = 8;
    const size_t shm_size = calculate_queue_size<int>(capacity);

    ShmHelper shm("/test_basic", shm_size);

    auto* queue = static_cast<ptorpis::spsc_queue_shm<int>*>(shm.get());

    queue->init(capacity);

    EXPECT_TRUE(queue->try_push(42));

    int value;
    EXPECT_TRUE(queue->try_pop(value));
    EXPECT_EQ(value, 42);

    // Queue should be empty now
    EXPECT_FALSE(queue->try_pop(value));
}

TEST(SPSCQueueShm, FillAndDrain) {
    const size_t capacity = 8;
    const size_t shm_size = calculate_queue_size<int>(capacity);

    ShmHelper shm("/test_fill", shm_size);
    auto* queue = static_cast<ptorpis::spsc_queue_shm<int>*>(shm.get());
    queue->init(capacity);

    // Fill to capacity (remember: capacity - 1 usable slots for 8)
    // bit_ceil(8 + 1) = 16, so we get 15 usable slots
    size_t usable = std::bit_ceil(capacity + 1) - 1;

    for (size_t i = 0; i < usable; ++i) {
        EXPECT_TRUE(queue->try_push(static_cast<int>(i))) << "Failed to push item " << i;
    }

    // Queue should be full
    EXPECT_FALSE(queue->try_push(999));

    // Drain completely
    for (size_t i = 0; i < usable; ++i) {
        int value;
        EXPECT_TRUE(queue->try_pop(value)) << "Failed to pop item " << i;
        EXPECT_EQ(value, static_cast<int>(i));
    }

    // Queue should be empty
    int value;
    EXPECT_FALSE(queue->try_pop(value));
}

TEST(SPSCQueueShm, Wraparound) {
    const size_t capacity = 8;
    const size_t shm_size = calculate_queue_size<int>(capacity);

    ShmHelper shm("/test_wrap", shm_size);
    auto* queue = static_cast<ptorpis::spsc_queue_shm<int>*>(shm.get());
    queue->init(capacity);

    // Push and pop repeatedly to force wraparound
    for (int cycle = 0; cycle < 100; ++cycle) {
        EXPECT_TRUE(queue->try_push(cycle));

        int value;
        EXPECT_TRUE(queue->try_pop(value));
        EXPECT_EQ(value, cycle);
    }
}

TEST(SPSCQueueShm, StructType) {
    struct Message {
        int id;
        char text[64];
        double timestamp;
    };

    const size_t capacity = 8;
    const size_t shm_size = calculate_queue_size<Message>(capacity);

    ShmHelper shm("/test_struct", shm_size);
    auto* queue = static_cast<ptorpis::spsc_queue_shm<Message>*>(shm.get());

    queue->init(capacity);

    Message msg_in{42, "Hello, World!", 123.456};
    EXPECT_TRUE(queue->try_push(msg_in));

    Message msg_out;
    EXPECT_TRUE(queue->try_pop(msg_out));

    EXPECT_EQ(msg_out.id, 42);
    EXPECT_STREQ(msg_out.text, "Hello, World!");
    EXPECT_DOUBLE_EQ(msg_out.timestamp, 123.456);
}

TEST(SPSCQueueShm, EmptyPopFails) {
    const size_t capacity = 8;
    const size_t shm_size = calculate_queue_size<int>(capacity);

    ShmHelper shm("/test_empty", shm_size);
    auto* queue = static_cast<ptorpis::spsc_queue_shm<int>*>(shm.get());
    queue->init(capacity);

    int value;
    EXPECT_FALSE(queue->try_pop(value));
}

// Multi-Process Tests

TEST(SPSCQueueShm, TwoProcessBasic) {
    const size_t capacity = 256;
    const size_t shm_size = calculate_queue_size<int>(capacity);
    const char* shm_name = "/test_two_process";

    // Parent creates shared memory
    ShmHelper shm(shm_name, shm_size);
    auto* queue = static_cast<ptorpis::spsc_queue_shm<int>*>(shm.get());
    queue->init(capacity);

    pid_t pid = fork();
    ASSERT_NE(pid, -1) << "fork() failed";

    if (pid == 0) {
        // Child process (consumer)
        void* ptr = ShmHelper::open(shm_name, shm_size);
        ASSERT_NE(ptr, nullptr);

        auto* child_queue = static_cast<ptorpis::spsc_queue_shm<int>*>(ptr);

        // Receive 100 items
        for (int i = 0; i < 100; ++i) {
            int value;
            while (!child_queue->try_pop(value)) {
                std::this_thread::yield();
            }

            if (value != i) {
                std::exit(1); // Failure
            }
        }

        munmap(ptr, shm_size);
        std::exit(0); // Success
    } else {
        // Parent process (producer)
        for (int i = 0; i < 100; ++i) {
            while (!queue->try_push(i)) {
                std::this_thread::yield();
            }
        }

        // Wait for child
        int status;
        waitpid(pid, &status, 0);
        EXPECT_EQ(WEXITSTATUS(status), 0) << "Child process failed";
    }
}

TEST(SPSCQueueShm, HighThroughput) {
    const size_t capacity = 1024;
    const size_t shm_size = calculate_queue_size<int>(capacity);
    const char* shm_name = "/test_throughput";
    const int NUM_ITEMS = 100000;

    ShmHelper shm(shm_name, shm_size);
    auto* queue = static_cast<ptorpis::spsc_queue_shm<int>*>(shm.get());
    queue->init(capacity);

    pid_t pid = fork();
    ASSERT_NE(pid, -1);

    if (pid == 0) {
        // Child (consumer)
        void* ptr = ShmHelper::open(shm_name, shm_size);
        auto* child_queue = static_cast<ptorpis::spsc_queue_shm<int>*>(ptr);

        int expected = 0;
        while (expected < NUM_ITEMS) {
            int value;
            if (child_queue->try_pop(value)) {
                if (value != expected) {
                    std::exit(1); // Data corruption
                }
                ++expected;
            } else {
                std::this_thread::yield();
            }
        }

        munmap(ptr, shm_size);
        std::exit(0);
    } else {
        // Parent (producer)
        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < NUM_ITEMS; ++i) {
            while (!queue->try_push(i)) {
                std::this_thread::yield();
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // Wait for consumer
        int status;
        waitpid(pid, &status, 0);
        EXPECT_EQ(WEXITSTATUS(status), 0);

        double throughput = NUM_ITEMS * 1000.0 / duration.count();
        std::cout << "Throughput: " << throughput << " items/sec\n";
        std::cout << "Duration: " << duration.count() << " ms\n";
    }
}

TEST(SPSCQueueShm, BurstyTraffic) {
    const size_t capacity = 64;
    const size_t shm_size = calculate_queue_size<int>(capacity);
    const char* shm_name = "/test_bursty";
    const int NUM_BURSTS = 100;
    const int BURST_SIZE = 50;

    ShmHelper shm(shm_name, shm_size);
    auto* queue = static_cast<ptorpis::spsc_queue_shm<int>*>(shm.get());
    queue->init(capacity);

    pid_t pid = fork();
    ASSERT_NE(pid, -1);

    if (pid == 0) {
        // Child (consumer)
        void* ptr = ShmHelper::open(shm_name, shm_size);
        auto* child_queue = static_cast<ptorpis::spsc_queue_shm<int>*>(ptr);

        int expected = 0;
        int total = NUM_BURSTS * BURST_SIZE;

        while (expected < total) {
            int value;
            if (child_queue->try_pop(value)) {
                if (value != expected) {
                    std::exit(1);
                }
                ++expected;
            }
        }

        munmap(ptr, shm_size);
        std::exit(0);
    } else {
        // Parent (producer) - send in bursts
        for (int burst = 0; burst < NUM_BURSTS; ++burst) {
            // Send burst
            for (int i = 0; i < BURST_SIZE; ++i) {
                int value = burst * BURST_SIZE + i;
                while (!queue->try_push(value)) {
                    std::this_thread::yield();
                }
            }

            // Pause between bursts
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        int status;
        waitpid(pid, &status, 0);
        EXPECT_EQ(WEXITSTATUS(status), 0);
    }
}

TEST(SPSCQueueShm, ProducerFasterThanConsumer) {
    const size_t capacity = 32;
    const size_t shm_size = calculate_queue_size<int>(capacity);
    const char* shm_name = "/test_producer_fast";
    const int NUM_ITEMS = 1000;

    ShmHelper shm(shm_name, shm_size);
    auto* queue = static_cast<ptorpis::spsc_queue_shm<int>*>(shm.get());
    queue->init(capacity);

    pid_t pid = fork();
    ASSERT_NE(pid, -1);

    if (pid == 0) {
        // Child (slow consumer)
        void* ptr = ShmHelper::open(shm_name, shm_size);
        auto* child_queue = static_cast<ptorpis::spsc_queue_shm<int>*>(ptr);

        for (int i = 0; i < NUM_ITEMS; ++i) {
            int value;
            while (!child_queue->try_pop(value)) {
                std::this_thread::yield();
            }

            if (value != i) {
                std::exit(1);
            }

            // Slow down consumer
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

        munmap(ptr, shm_size);
        std::exit(0);
    } else {
        // Parent (fast producer)
        for (int i = 0; i < NUM_ITEMS; ++i) {
            while (!queue->try_push(i)) {
                std::this_thread::yield();
            }
        }

        int status;
        waitpid(pid, &status, 0);
        EXPECT_EQ(WEXITSTATUS(status), 0);
    }
}

TEST(SPSCQueueShm, ConsumerFasterThanProducer) {
    const size_t capacity = 32;
    const size_t shm_size = calculate_queue_size<int>(capacity);
    const char* shm_name = "/test_consumer_fast";
    const int NUM_ITEMS = 1000;

    ShmHelper shm(shm_name, shm_size);
    auto* queue = static_cast<ptorpis::spsc_queue_shm<int>*>(shm.get());
    queue->init(capacity);

    pid_t pid = fork();
    ASSERT_NE(pid, -1);

    if (pid == 0) {
        // Child (fast consumer)
        void* ptr = ShmHelper::open(shm_name, shm_size);
        auto* child_queue = static_cast<ptorpis::spsc_queue_shm<int>*>(ptr);

        for (int i = 0; i < NUM_ITEMS; ++i) {
            int value;
            while (!child_queue->try_pop(value)) {
                // Spin fast
            }

            if (value != i) {
                std::exit(1);
            }
        }

        munmap(ptr, shm_size);
        std::exit(0);
    } else {
        // Parent (slow producer)
        for (int i = 0; i < NUM_ITEMS; ++i) {
            while (!queue->try_push(i)) {
                std::this_thread::yield();
            }

            // Slow down producer
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }

        int status;
        waitpid(pid, &status, 0);
        EXPECT_EQ(WEXITSTATUS(status), 0);
    }
}
