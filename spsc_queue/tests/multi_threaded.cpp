#include "spsc-queue.hpp"
#include <gtest/gtest.h>

TEST(SPSCQueue, ConcurrentStressTest) {
    ptorpis::spsc_queue<int> q(1024);
    const int NUM_ITEMS = 1000000;
    std::atomic<bool> producer_done{false};

    // Producer thread
    std::thread producer([&q, &producer_done]() {
        for (int i = 0; i < NUM_ITEMS; ++i) {
            while (!q.try_push(i)) {
                std::this_thread::yield();
            }
        }
        producer_done = true;
    });

    // Consumer thread
    std::thread consumer([&q]() {
        int expected = 0;
        while (expected < NUM_ITEMS) {
            int value;
            if (q.try_pop(value)) {
                EXPECT_EQ(value, expected);
                ++expected;
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();
    EXPECT_TRUE(q.empty());
}

TEST(SPSCQueue, HighContentionTest) {
    ptorpis::spsc_queue<int> q(16); // Small queue for high contention
    const int NUM_ITEMS = 100000;
    std::atomic<int> checksum_producer{0};
    std::atomic<int> checksum_consumer{0};

    std::thread producer([&]() {
        for (int i = 0; i < NUM_ITEMS; ++i) {
            while (!q.try_push(i)) {
                // Busy spin - creates contention
            }
            checksum_producer += i;
        }
    });

    std::thread consumer([&]() {
        for (int i = 0; i < NUM_ITEMS; ++i) {
            int value;
            while (!q.try_pop(value)) {
                // Busy spin
            }
            checksum_consumer += value;
        }
    });

    producer.join();
    consumer.join();

    // Verify no data loss or corruption
    EXPECT_EQ(checksum_producer.load(), checksum_consumer.load());
}

TEST(SPSCQueue, BurstyTraffic) {
    ptorpis::spsc_queue<int> q(256);
    const int NUM_BURSTS = 1000;
    const int BURST_SIZE = 100;

    std::thread producer([&]() {
        for (int burst = 0; burst < NUM_BURSTS; ++burst) {
            // Push burst
            for (int i = 0; i < BURST_SIZE; ++i) {
                while (!q.try_push(burst * BURST_SIZE + i)) {
                    std::this_thread::yield();
                }
            }
            // Pause between bursts
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });

    std::thread consumer([&]() {
        int expected = 0;
        while (expected < NUM_BURSTS * BURST_SIZE) {
            int value;
            if (q.try_pop(value)) {
                EXPECT_EQ(value, expected);
                ++expected;
            } else {
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();
}
