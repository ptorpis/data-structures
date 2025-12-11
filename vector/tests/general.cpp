#include "vector.hpp"
#include <gtest/gtest.h>
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
