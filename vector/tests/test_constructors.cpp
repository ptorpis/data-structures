#include "vector.hpp"
#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

TEST(VectorTest, DefaultConstructor) {
    ptorpis::vector<int> v;

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, CountConstructorBase) {
    unsigned int n{5};
    ptorpis::vector<int> v(n);

    EXPECT_EQ(v.size(), n);
    EXPECT_EQ(v.capacity(), n);
    EXPECT_FALSE(v.empty());

    for (std::size_t i{}; i < n; ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

TEST(VectorTest, SizeComp) {
    unsigned int n = 3;
    std::vector<int> stdVec(n);
    ptorpis::vector<int> myVec(n);

    EXPECT_EQ(sizeof(stdVec), sizeof(myVec));
}

TEST(VectorTest, CountValueConstructor) {
    ptorpis::vector<int> v(5, 42);

    for (std::size_t i{}; i < 5; ++i) {
        EXPECT_EQ(v[i], 42);
    }
}

TEST(VectorTest, InitializerTest) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    std::vector<int> stdVec{1, 2, 3, 4, 5};

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
    EXPECT_FALSE(v.empty());

    for (std::size_t i{}; i < 5; ++i) {
        EXPECT_EQ(v[i], i + 1);

        EXPECT_EQ(stdVec[i], v[i]);
    }
}
