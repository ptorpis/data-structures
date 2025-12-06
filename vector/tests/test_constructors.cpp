#include "vector.hpp"
#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

TEST(VectorConstructorTest, DefaultConstructor) {
    ptorpis::vector<int> v;

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.data(), nullptr);
}

TEST(VectorConstructorTest, CountConstructorBase) {
    unsigned int n{5};
    ptorpis::vector<int> v(n);

    EXPECT_EQ(v.size(), n);
    EXPECT_EQ(v.capacity(), n);
    EXPECT_FALSE(v.empty());

    for (std::size_t i{}; i < n; ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

TEST(VectorConstructorTest, ConstructorZeroCount) {
    ptorpis::vector<int> v(0);

    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorConstructorTest, SizeComp) {
    unsigned int n{3};
    std::vector<int> stdVec(n);
    ptorpis::vector<int> myVec(n);

    EXPECT_EQ(sizeof(stdVec), sizeof(myVec));
}

TEST(VectorConstructorTest, CountValueConstructor) {
    unsigned int n{5};
    int val{42};
    ptorpis::vector<int> v(n, val);

    for (std::size_t i{}; i < n; ++i) {
        EXPECT_EQ(v[i], val);
    }
}

TEST(VectorConstructorTest, InitializerListConstuructor) {
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

TEST(VectorConstructorTest, InitializerListEmpty) {
    ptorpis::vector<int> v{};

    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorConstructorTest, InitializerListSingleElement) {
    ptorpis::vector<int> v{42};

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 42);
}

TEST(VectorConstructorTest, CountValueConstructorZeroCount) {
    ptorpis::vector<int> v(0, 42);

    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
}

TEST(VectorConstructorTest, CopyConstructor) {
    ptorpis::vector<int> v1{1, 2, 3, 4, 5};
    ptorpis::vector<int> v2(v1);

    EXPECT_EQ(v2.size(), v1.size());
    EXPECT_EQ(v2.size(), 5);

    for (size_t i = 0; i < v1.size(); ++i) {
        EXPECT_EQ(v2[i], v1[i]);
    }
}

TEST(VectorConstructorTest, CopyConstructorDeepCopy) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2(v1);

    v1[0] = 999;

    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v1[0], 999);
}

TEST(VectorConstructorTest, CopyConstructorEmpty) {
    ptorpis::vector<int> v1;
    ptorpis::vector<int> v2(v1);

    EXPECT_EQ(v2.size(), 0);
    EXPECT_TRUE(v2.empty());
}

TEST(VectorConstructorTest, CopyConstructorIndependentMemory) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2(v1);

    EXPECT_NE(v1.data(), v2.data());
}
