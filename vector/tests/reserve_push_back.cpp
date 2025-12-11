#include "vector.hpp"
#include <gtest/gtest.h>
#include <string>

/*
 * reserve()
 */

TEST(VectorReserveTest, ReserveOnEmpty) {
    ptorpis::vector<int> v;

    v.reserve(10);

    EXPECT_EQ(v.size(), 0);
    EXPECT_GE(v.capacity(), 10);
    EXPECT_TRUE(v.empty());
}

TEST(VectorReserveTest, ReserveLargeCapacity) {
    ptorpis::vector<int> v;

    v.reserve(1000);

    EXPECT_EQ(v.size(), 0);
    EXPECT_GE(v.capacity(), 1000);
}

TEST(VectorReserveTest, ReserveDoesNotChangeSize) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    size_t old_size = v.size();

    v.reserve(100);

    EXPECT_EQ(v.size(), old_size);
    EXPECT_GE(v.capacity(), 100);
}

TEST(VectorReserveTest, ReservePreservesElements) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    v.reserve(100);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorReserveTest, ReserveDoesNotShrink) {
    ptorpis::vector<int> v;
    v.reserve(100);
    size_t old_capacity = v.capacity();

    v.reserve(10); // Try to reserve less

    EXPECT_EQ(v.capacity(), old_capacity); // Capacity unchanged
}

TEST(VectorReserveTest, ReserveSameCapacity) {
    ptorpis::vector<int> v;
    v.reserve(50);
    size_t old_capacity = v.capacity();

    v.reserve(50); // Reserve same amount

    EXPECT_EQ(v.capacity(), old_capacity); // No change
}

TEST(VectorReserveTest, ReserveInvalidatesPointers) {
    ptorpis::vector<int> v{1, 2, 3};
    int* old_data = v.data();

    v.reserve(100);

    EXPECT_NE(v.data(), old_data); // Pointer changed
    EXPECT_EQ(v[0], 1);            // But data preserved
}

TEST(VectorReserveTest, ReserveWithStrings) {
    ptorpis::vector<std::string> v{"hello", "world"};

    v.reserve(100);

    EXPECT_EQ(v.size(), 2);
    EXPECT_GE(v.capacity(), 100);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
}

TEST(VectorReserveTest, ReserveZero) {
    ptorpis::vector<int> v{1, 2, 3};
    size_t old_capacity = v.capacity();

    v.reserve(0); // Should do nothing

    EXPECT_EQ(v.capacity(), old_capacity);
    EXPECT_EQ(v.size(), 3);
}

TEST(VectorReserveTest, MultipleReserves) {
    ptorpis::vector<int> v;

    v.reserve(10);
    EXPECT_GE(v.capacity(), 10);

    v.reserve(50);
    EXPECT_GE(v.capacity(), 50);

    v.reserve(100);
    EXPECT_GE(v.capacity(), 100);
}

/*
 * push_back()
 */

TEST(VectorPushBackTest, PushBackToEmpty) {
    ptorpis::vector<int> v;

    v.push_back(42);

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
    EXPECT_GE(v.capacity(), 1);
}

TEST(VectorPushBackTest, PushBackMultiple) {
    ptorpis::vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorPushBackTest, PushBackMany) {
    ptorpis::vector<int> v;

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    EXPECT_EQ(v.size(), 100);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(v[i], i);
    }
}

TEST(VectorPushBackTest, PushBackStrings) {
    ptorpis::vector<std::string> v;

    v.push_back("hello");
    v.push_back("world");
    v.push_back("test");

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
    EXPECT_EQ(v[2], "test");
}

TEST(VectorPushBackTest, PushBackLvalue) {
    ptorpis::vector<std::string> v;
    std::string s = "hello";

    v.push_back(s); // Copy

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(s, "hello"); // Original unchanged
}

TEST(VectorPushBackTest, PushBackRvalue) {
    ptorpis::vector<std::string> v;
    std::string s = "hello";

    v.push_back(std::move(s)); // Move

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "hello");
    EXPECT_TRUE(s.empty() || s == "hello"); // Moved-from state
}

TEST(VectorPushBackTest, PushBackTemporary) {
    ptorpis::vector<std::string> v;

    v.push_back("temporary"); // Rvalue, should move

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "temporary");
}

TEST(VectorPushBackTest, GrowthFromZero) {
    ptorpis::vector<int> v;

    v.push_back(1);
    EXPECT_EQ(v.capacity(), 1); // First allocation

    v.push_back(2);
    EXPECT_EQ(v.capacity(), 2); // Doubled

    v.push_back(3);
    EXPECT_EQ(v.capacity(), 4); // Doubled again
}

TEST(VectorPushBackTest, GeometricGrowth) {
    ptorpis::vector<int> v;
    std::vector<size_t> capacities;

    for (int i = 0; i < 20; ++i) {
        size_t old_cap = v.capacity();
        v.push_back(i);
        if (v.capacity() != old_cap) {
            capacities.push_back(v.capacity());
        }
    }

    // Should see geometric growth: 1, 2, 4, 8, 16, 32...
    EXPECT_FALSE(capacities.empty());
    for (size_t i = 1; i < capacities.size(); ++i) {
        // Each capacity should be at least 1.5x the previous
        EXPECT_GE(capacities[i], capacities[i - 1] * 1.5);
    }
}

TEST(VectorPushBackTest, NoReallocationWhenCapacityAvailable) {
    ptorpis::vector<int> v;
    v.reserve(10);

    int* old_data = v.data();

    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }

    EXPECT_EQ(v.data(), old_data); // No reallocation occurred
    EXPECT_EQ(v.size(), 10);
}

TEST(VectorPushBackTest, ReallocationOnCapacityExceeded) {
    ptorpis::vector<int> v;
    v.reserve(5);

    for (int i = 0; i < 5; ++i) {
        v.push_back(i);
    }

    int* old_data = v.data();
    size_t old_capacity = v.capacity();

    v.push_back(5); // Should trigger reallocation

    EXPECT_NE(v.data(), old_data);         // Pointer changed
    EXPECT_GT(v.capacity(), old_capacity); // Capacity increased
    EXPECT_EQ(v.size(), 6);
}

TEST(VectorPushBackTest, PreservesExistingElements) {
    ptorpis::vector<int> v{1, 2, 3};

    v.push_back(4);
    v.push_back(5);

    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorPushBackTest, PreservesAfterReallocation) {
    ptorpis::vector<int> v;

    // Add elements to trigger multiple reallocations
    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    // Verify all elements preserved
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(v[i], i);
    }
}

TEST(VectorPushBackTest, PreservesStringContent) {
    ptorpis::vector<std::string> v;

    for (int i = 0; i < 50; ++i) {
        v.push_back("string_" + std::to_string(i));
    }

    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(v[i], "string_" + std::to_string(i));
    }
}

TEST(VectorPushBackTest, PushBackAfterCopy) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2(v1);

    v2.push_back(4);

    EXPECT_EQ(v2.size(), 4);
    EXPECT_EQ(v2[3], 4);
    EXPECT_EQ(v1.size(), 3); // Original unchanged
}

TEST(VectorPushBackTest, PushBackAfterMove) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2(std::move(v1));

    v2.push_back(4);

    EXPECT_EQ(v2.size(), 4);
    EXPECT_EQ(v2[3], 4);
}

TEST(VectorPushBackTest, PushBackAfterAssignment) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2;

    v2 = v1;
    v2.push_back(4);

    EXPECT_EQ(v2.size(), 4);
    EXPECT_EQ(v1.size(), 3);
}

TEST(VectorPushBackTest, MixReserveAndPushBack) {
    ptorpis::vector<int> v;

    v.reserve(5);
    v.push_back(1);
    v.push_back(2);

    v.reserve(10);
    v.push_back(3);
    v.push_back(4);

    EXPECT_EQ(v.size(), 4);
    EXPECT_GE(v.capacity(), 10);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[3], 4);
}

TEST(VectorPushBackTest, AmortizedConstantTime) {
    ptorpis::vector<int> v;
    size_t reallocation_count = 0;

    int* last_data = nullptr;
    for (int i = 0; i < 1000; ++i) {
        int* current_data = v.data();
        v.push_back(i);
        if (current_data != last_data && last_data != nullptr) {
            ++reallocation_count;
        }
        last_data = v.data();
    }

    // With geometric growth, should have ~log2(1000) ≈ 10 reallocations
    EXPECT_LT(reallocation_count, 20); // Much less than 1000
}

TEST(VectorPushBackTest, NoUnnecessaryReallocations) {
    ptorpis::vector<int> v;
    v.reserve(100);

    int* data_ptr = v.data();

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }

    EXPECT_EQ(v.data(), data_ptr); // No reallocations
}

// Combined reserve() and push_back() Tests

TEST(VectorCombinedTest, ReserveThenPushBack) {
    ptorpis::vector<int> v;

    v.reserve(10);
    size_t capacity_after_reserve = v.capacity();

    for (int i = 0; i < 5; ++i) {
        v.push_back(i);
    }

    EXPECT_EQ(v.capacity(), capacity_after_reserve); // No growth
    EXPECT_EQ(v.size(), 5);
}

TEST(VectorCombinedTest, PushBackThenReserve) {
    ptorpis::vector<int> v;

    for (int i = 0; i < 5; ++i) {
        v.push_back(i);
    }

    v.reserve(100);

    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 100);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], i);
    }
}

TEST(VectorCombinedTest, AlternatingReserveAndPushBack) {
    ptorpis::vector<int> v;

    v.reserve(2);
    v.push_back(1);
    v.push_back(2);

    v.reserve(5);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);

    EXPECT_EQ(v.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(v[i], i + 1);
    }
}

TEST(VectorEdgeCaseTest, PushBackToVectorOfVectors) {
    ptorpis::vector<ptorpis::vector<int>> vv;

    ptorpis::vector<int> v1{1, 2, 3};
    vv.push_back(v1);

    ptorpis::vector<int> v2{4, 5, 6};
    vv.push_back(std::move(v2));

    EXPECT_EQ(vv.size(), 2);
    EXPECT_EQ(vv[0].size(), 3);
    EXPECT_EQ(vv[1].size(), 3);
}

TEST(VectorEdgeCaseTest, LargeNumberOfPushBacks) {
    ptorpis::vector<int> v;

    for (int i = 0; i < 10000; ++i) {
        v.push_back(i);
    }

    EXPECT_EQ(v.size(), 10000);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[9999], 9999);
}
