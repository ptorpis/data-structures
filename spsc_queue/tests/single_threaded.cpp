#include "spsc_queue.hpp"
#include <gtest/gtest.h>

TEST(SPSCQueue, BasicPushPop) {
    ptorpis::spsc_queue<int> q(8);
    EXPECT_TRUE(q.empty());
    EXPECT_TRUE(q.try_push(42));
    EXPECT_FALSE(q.empty());

    int value;
    EXPECT_TRUE(q.try_pop(value));
    EXPECT_EQ(value, 42);
    EXPECT_TRUE(q.empty());
}

TEST(SPSCQueue, Capacity) {
    ptorpis::spsc_queue<int> q(7);

    EXPECT_EQ(q.capacity(), 7);
}

TEST(SPSCQueue, LargeCapacity) {
    ptorpis::spsc_queue<int> q(1023);

    EXPECT_EQ(q.capacity(), 1023);
}

TEST(SPSCQueue, FillAndDrain) {
    ptorpis::spsc_queue<int> q(8);

    for (int i = 0; i < 7; ++i) {
        EXPECT_TRUE(q.try_push(i));
    }
    EXPECT_TRUE(q.full());
    EXPECT_FALSE(q.try_push(999)); // Should fail

    for (int i = 0; i < 7; ++i) {
        int value;
        EXPECT_TRUE(q.try_pop(value));
        EXPECT_EQ(value, i);
    }
    EXPECT_TRUE(q.empty());
    int val;
    EXPECT_FALSE(q.try_pop(val)); // Should fail
}

TEST(SPSCQueue, Wraparound) {
    ptorpis::spsc_queue<int> q(8);

    for (int cycle = 1; cycle < 100; ++cycle) {
        EXPECT_TRUE(q.try_push(cycle));
        int value;
        EXPECT_TRUE(q.try_pop(value));
        EXPECT_EQ(value, cycle);
    }
}

TEST(SPSCQueue, MoveSemantics) {
    ptorpis::spsc_queue<std::unique_ptr<int>> q(8);

    auto ptr = std::make_unique<int>(42);
    EXPECT_TRUE(q.try_push(std::move(ptr)));
    EXPECT_EQ(ptr, nullptr); // Moved from

    std::unique_ptr<int> result;
    EXPECT_TRUE(q.try_pop(result));
    EXPECT_EQ(*result, 42);
}
