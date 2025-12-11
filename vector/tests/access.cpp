#include "vector.hpp"
#include <gtest/gtest.h>

TEST(VectorAccessTest, OperatorBracket) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    std::vector<int> stdvec{1, 2, 3, 4, 5};

    for (std::size_t i{}; i < v.size(); ++i) {
        EXPECT_EQ(v[i], stdvec[i]);
    }
}

TEST(VectorAccessTest, OperatorBracketModify) {
    ptorpis::vector<int> v{10, 20, 30};

    v[0] = 100;
    v[1] = 200;
    v[2] = 300;

    EXPECT_EQ(v[0], 100);
    EXPECT_EQ(v[1], 200);
    EXPECT_EQ(v[2], 300);
}

TEST(VectorAccessTest, OperatorBracketConst) {
    const ptorpis::vector<int> v{10, 20, 30};

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorAccessTest, At) {
    ptorpis::vector<int> v{10, 20, 30, 40, 50};

    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(2), 30);
    EXPECT_EQ(v.at(4), 50);
}

TEST(VectorAccessTest, AtModify) {
    ptorpis::vector<int> v{10, 20, 30};

    v.at(0) = 100;
    v.at(1) = 200;

    EXPECT_EQ(v.at(0), 100);
    EXPECT_EQ(v.at(1), 200);
}

TEST(VectorAccessTest, AtConst) {
    const ptorpis::vector<int> v{10, 20, 30};

    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(1), 20);
    EXPECT_EQ(v.at(2), 30);
}

TEST(VectorAccessTest, AtThrowsOutOfRange) {
    ptorpis::vector<int> v{10, 20, 30};

    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_THROW(v.at(10), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(VectorAccessTest, AtThrowsOnEmpty) {
    ptorpis::vector<int> v;

    EXPECT_THROW(v.at(0), std::out_of_range);
}

TEST(VectorAccessTest, Front) {
    ptorpis::vector<int> v{10, 20, 30};

    EXPECT_EQ(v.front(), 10);
}

TEST(VectorAccessTest, FrontModify) {
    ptorpis::vector<int> v{10, 20, 30};

    v.front() = 100;

    EXPECT_EQ(v.front(), 100);
    EXPECT_EQ(v[0], 100);
}

TEST(VectorAccessTest, FrontConst) {
    const ptorpis::vector<int> v{10, 20, 30};

    EXPECT_EQ(v.front(), 10);
}

TEST(VectorAccessTest, Back) {
    ptorpis::vector<int> v{10, 20, 30};

    EXPECT_EQ(v.back(), 30);
}

TEST(VectorAccessTest, BackModify) {
    ptorpis::vector<int> v{10, 20, 30};

    v.back() = 300;

    EXPECT_EQ(v.back(), 300);
    EXPECT_EQ(v[2], 300);
}

TEST(VectorAccessTest, BackConst) {
    const ptorpis::vector<int> v{10, 20, 30};

    EXPECT_EQ(v.back(), 30);
}

TEST(VectorAccessTest, BackSingleElement) {
    ptorpis::vector<int> v{42};

    EXPECT_EQ(v.front(), 42);
    EXPECT_EQ(v.back(), 42);
}

TEST(VectorAccessTest, Data) {
    ptorpis::vector<int> v{10, 20, 30};

    int* ptr = v.data();

    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 20);
    EXPECT_EQ(ptr[2], 30);
}

TEST(VectorAccessTest, DataModify) {
    ptorpis::vector<int> v{10, 20, 30};

    int* ptr = v.data();
    ptr[0] = 100;

    EXPECT_EQ(v[0], 100);
}

TEST(VectorAccessTest, DataConst) {
    const ptorpis::vector<int> v{10, 20, 30};

    const int* ptr = v.data();

    EXPECT_EQ(ptr[0], 10);
    EXPECT_EQ(ptr[1], 20);
    EXPECT_EQ(ptr[2], 30);
}

TEST(VectorAccessTest, DataEmpty) {
    ptorpis::vector<int> v;

    EXPECT_EQ(v.data(), nullptr);
}
