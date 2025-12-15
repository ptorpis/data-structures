#include "vector.hpp"
#include <gtest/gtest.h>
#include <print>
#include <stdexcept>

TEST(VectorTest, PopBackBasic) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    v.pop_back();

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[3], 4); // Last element is now 4
}

TEST(VectorTest, PopBackUntilEmpty) {
    ptorpis::vector<int> v{1, 2, 3};

    v.pop_back();
    EXPECT_EQ(v.size(), 2);

    v.pop_back();
    EXPECT_EQ(v.size(), 1);

    v.pop_back();
    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, PopBackDoesNotReduceCapacity) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    size_t old_capacity = v.capacity();

    v.pop_back();

    EXPECT_EQ(v.capacity(), old_capacity);
}

TEST(VectorTest, PopBackDestroysCalled) {
    // This test relies on ASan to detect if destructor wasn't called
    ptorpis::vector<std::string> v{"hello", "world"};

    v.pop_back(); // String destructor should be called

    EXPECT_EQ(v.size(), 1);
}

TEST(VectorTest, ClearBasic) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    v.clear();

    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, ClearDoesNotReduceCapacity) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    size_t old_capacity = v.capacity();

    v.clear();

    EXPECT_EQ(v.capacity(), old_capacity);
}

TEST(VectorTest, ClearOnEmpty) {
    ptorpis::vector<int> v;

    v.clear();

    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, PushBackAfterClear) {
    ptorpis::vector<int> v{1, 2, 3};

    v.clear();
    v.push_back(10);
    v.push_back(20);

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
}

TEST(VectorTest, ClearDestroysElements) {
    // This test also relies on ASan
    ptorpis::vector<std::string> v{"hello", "world", "test"};

    v.clear(); // Should call destructors

    EXPECT_EQ(v.size(), 0);
}

TEST(VectorTest, AccessAfterClear) {
    ptorpis::vector<int> v{1};

    v.clear();

    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST(VectorTest, EmplaceBackBasic) {
    ptorpis::vector<std::string> v;

    v.emplace_back("hello");

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], "hello");
}

TEST(VectorTest, EmplaceBackMultipleArgs) {
    ptorpis::vector<std::string> v;

    v.emplace_back(5, 'x'); // Construct string(5, 'x')

    EXPECT_EQ(v[0], "xxxxx");
}

TEST(VectorTest, EmplaceBackReturnsReference) {
    ptorpis::vector<int> v;

    int& ref = v.emplace_back(42);

    EXPECT_EQ(ref, 42);
    ref = 99;
    EXPECT_EQ(v[0], 99); // Modified through reference
}

TEST(VectorTest, EmplaceBackWithGrowth) {
    ptorpis::vector<std::string> v;

    for (int i = 0; i < 100; ++i) {
        v.emplace_back(std::to_string(i));
    }

    EXPECT_EQ(v.size(), 100);
    EXPECT_EQ(v[50], "50");
}

// erase() - Single Element Tests

TEST(VectorEraseTest, EraseMiddleElement) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.begin() + 2); // Erase element at index 2 (value 3)

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 4); // 4 shifted left
    EXPECT_EQ(v[3], 5); // 5 shifted left

    EXPECT_EQ(*it, 4);
    EXPECT_EQ(it, v.begin() + 2);
}

TEST(VectorEraseTest, EraseFirstElement) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.begin());

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 4);
    EXPECT_EQ(v[3], 5);

    EXPECT_EQ(*it, 2);
    EXPECT_EQ(it, v.begin());
}

TEST(VectorEraseTest, EraseLastElement) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.begin() + 4); // Erase last element

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);

    EXPECT_EQ(it, v.end());
}

TEST(VectorEraseTest, EraseSingleElementVector) {
    ptorpis::vector<int> v{42};

    auto it = v.erase(v.begin());

    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(it, v.end());
}

TEST(VectorEraseTest, EraseWithStrings) {
    ptorpis::vector<std::string> v{"one", "two", "three", "four"};

    v.erase(v.begin() + 1); // Erase "two"

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], "one");
    EXPECT_EQ(v[1], "three");
    EXPECT_EQ(v[2], "four");
}

TEST(VectorEraseTest, EraseMultipleTimes) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    v.erase(v.begin() + 2); // Erase 3: {1, 2, 4, 5}
    EXPECT_EQ(v.size(), 4);

    v.erase(v.begin()); // Erase 1: {2, 4, 5}
    EXPECT_EQ(v.size(), 3);

    v.erase(v.end() - 1); // Erase 5: {2, 4}
    EXPECT_EQ(v.size(), 2);

    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[1], 4);
}

TEST(VectorEraseTest, EraseDoesNotChangeCapacity) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    size_t old_capacity = v.capacity();

    v.erase(v.begin() + 2);

    EXPECT_EQ(v.capacity(), old_capacity); // Capacity unchanged
}

TEST(VectorEraseTest, EraseUsingConstIterator) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.cbegin() + 2); // Use const_iterator

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(*it, 4);
}

// erase() - Range Tests

TEST(VectorEraseTest, EraseRangeMiddle) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5, 6, 7};

    auto it = v.erase(v.begin() + 2, v.begin() + 5); // Erase [2, 5): indices 2,3,4

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 6); // 6 shifted to position 2
    EXPECT_EQ(v[3], 7); // 7 shifted to position 3

    EXPECT_EQ(*it, 6);
    EXPECT_EQ(it, v.begin() + 2);
}

TEST(VectorEraseTest, EraseRangeFromBeginning) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.begin(), v.begin() + 3); // Erase first 3

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 4);
    EXPECT_EQ(v[1], 5);

    EXPECT_EQ(*it, 4);
}

TEST(VectorEraseTest, EraseRangeToEnd) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.begin() + 2, v.end()); // Erase last 3

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);

    EXPECT_EQ(it, v.end());
}

TEST(VectorEraseTest, EraseEntireRange) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.begin(), v.end());

    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(it, v.end());
}

TEST(VectorEraseTest, EraseEmptyRange) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.begin() + 2, v.begin() + 2); // Empty range

    EXPECT_EQ(v.size(), 5); // Nothing erased
    EXPECT_EQ(it, v.begin() + 2);
}

TEST(VectorEraseTest, EraseRangeSingleElement) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    v.erase(v.begin() + 2, v.begin() + 3); // Erase just index 2

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 4);
    EXPECT_EQ(v[3], 5);
}

TEST(VectorEraseTest, EraseRangeWithStrings) {
    ptorpis::vector<std::string> v{"a", "b", "c", "d", "e", "f"};

    v.erase(v.begin() + 1, v.begin() + 4); // Erase "b", "c", "d"

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], "a");
    EXPECT_EQ(v[1], "e");
    EXPECT_EQ(v[2], "f");
}

TEST(VectorEraseTest, EraseRangeMultipleTimes) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    v.erase(v.begin() + 2, v.begin() + 5); // Erase 3,4,5: {1,2,6,7,8,9,10}
    EXPECT_EQ(v.size(), 7);

    v.erase(v.begin(), v.begin() + 2); // Erase 1,2: {6,7,8,9,10}
    EXPECT_EQ(v.size(), 5);

    EXPECT_EQ(v[0], 6);
    EXPECT_EQ(v[4], 10);
}

// Iterator Validity Tests

TEST(VectorEraseTest, IteratorsAfterEraseAreValid) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.begin() + 2);

    // Iterator returned by erase should be valid
    EXPECT_EQ(*it, 4);

    // Can continue using it
    ++it;
    EXPECT_EQ(*it, 5);
}

TEST(VectorEraseTest, IteratorsBeforeEraseStillValid) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto before = v.begin() + 1;
    v.erase(v.begin() + 3); // Erase after 'before'

    EXPECT_EQ(*before, 2);
}

// Edge Cases

TEST(VectorEraseTest, EraseAndPushBack) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    v.erase(v.begin() + 2);
    v.push_back(6);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 4);
    EXPECT_EQ(v[3], 5);
    EXPECT_EQ(v[4], 6);
}

TEST(VectorEraseTest, EraseInLoop) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Erase all even numbers
    for (auto it = v.begin(); it != v.end();) {
        if (*it % 2 == 0) {
            it = v.erase(it);
        } else {
            ++it;
        }
    }

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 5);
    EXPECT_EQ(v[3], 7);
    EXPECT_EQ(v[4], 9);
}

TEST(VectorEraseTest, EraseAllElementsOneByOne) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    while (!v.empty()) {
        v.erase(v.begin());
    }

    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorEraseTest, EraseLargeRange) {
    ptorpis::vector<int> v;
    for (int i = 0; i < 1000; ++i) {
        v.push_back(i);
    }

    v.erase(v.begin() + 100, v.begin() + 900);

    EXPECT_EQ(v.size(), 200);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[100], 900);
}

// Comparison with std::vector

TEST(VectorEraseTest, BehaviorMatchesStdVector) {
    ptorpis::vector<int> v1{1, 2, 3, 4, 5};
    std::vector<int> v2{1, 2, 3, 4, 5};

    auto it1 = v1.erase(v1.begin() + 2);
    auto it2 = v2.erase(v2.begin() + 2);

    EXPECT_EQ(v1.size(), v2.size());
    EXPECT_EQ(*it1, *it2);

    for (size_t i = 0; i < v1.size(); ++i) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

// Memory Management Tests

TEST(VectorEraseTest, DestructorsCalled) {
    // This test relies on ASan/Valgrind to detect if destructors aren't called
    ptorpis::vector<std::string> v{"one", "two", "three", "four", "five"};

    v.erase(v.begin() + 1, v.begin() + 4);

    EXPECT_EQ(v.size(), 2);
    // ASan would report a leak
}

TEST(VectorEraseTest, EraseDoesNotLeak) {
    // This test relies on ASan/Valgrind to detect if destructors aren't called
    ptorpis::vector<std::string> v;
    for (int i = 0; i < 100; ++i) {
        v.push_back(std::to_string(i));
    }

    v.erase(v.begin(), v.begin() + 50);

    EXPECT_EQ(v.size(), 50);
}

// Type Traits Tests

TEST(VectorEraseTest, ReturnTypeIsIterator) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.erase(v.begin());

    static_assert(std::is_same_v<decltype(it), ptorpis::vector<int>::iterator>);
}

TEST(VectorTest, ShrinkToFitReducesCapacity) {
    ptorpis::vector<int> v;
    v.reserve(100);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_GE(v.capacity(), 100);

    v.shrink_to_fit();

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 3); // Or close to it
}

TEST(VectorTest, ShrinkToFitEmptyVector) {
    ptorpis::vector<int> v;
    v.reserve(100);

    v.shrink_to_fit();

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(VectorTest, ShrinkToFitAlreadyTight) {
    ptorpis::vector<int> v{1, 2, 3};
    size_t old_capacity = v.capacity();

    v.shrink_to_fit();

    EXPECT_EQ(v.capacity(), old_capacity); // No change needed
}

TEST(VectorTest, ShrinkToFitPreservesElements) {
    ptorpis::vector<int> v;
    v.reserve(100);
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }

    v.shrink_to_fit();

    EXPECT_EQ(v.size(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(v[i], i);
    }
}

TEST(VectorTest, MaxSize) {
    ptorpis::vector<int> v{};

    std::size_t maxSize = v.max_size();

    EXPECT_GT(maxSize, 100'000'000);
    EXPECT_THROW(v.reserve(maxSize + 1), std::length_error);
}

TEST(VectorTest, SizeParityAtLargerCounts) {
    ptorpis::vector<int> v(1000, 42);
    std::vector<int> sv(1000, 42);

    EXPECT_EQ(v.size(), sv.size());
    EXPECT_EQ(sizeof(v), sizeof(sv));
}
