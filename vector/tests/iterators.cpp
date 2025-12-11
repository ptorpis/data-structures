#include "iterators.hpp"
#include "vector.hpp"
#include <gtest/gtest.h>

TEST(VectorIteratorTest, RangeIteration) {
    ptorpis::vector<int> v(5);
    for (auto elem : v) {
        ASSERT_EQ(elem, 0);
    }
}

TEST(VectorIteratorTest, IteratorBegin) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    ptorpis::vector<int>::iterator it = v.begin();

    ASSERT_EQ(*it, 1);
}

TEST(VectorIteratorTest, IteratorPreIncrement) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    auto it = v.begin();

    ASSERT_EQ(*(++it), 2);
}

TEST(VectorIteratorTest, IteratorPostIncrement) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    auto it = v.begin();

    ASSERT_EQ(*(it++), 1);
    ASSERT_EQ(*it, 2);
}

TEST(VectorIteratorTest, IteratorPreDecrement) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    auto it = v.end();
    --it;

    ASSERT_EQ(*(--it), 4);
}

TEST(VectorIteratorTest, IteratorPostDecrement) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    auto it = v.end();
    --it;

    ASSERT_EQ(*(it--), 5);
    ASSERT_EQ(*it, 4);
}

TEST(VectorIteratorTest, IteratorBracketAccess) {
    ptorpis::vector<int> v{0, 1, 2, 3, 4};
    auto it = v.begin();

    for (std::size_t i{}; i < 5; ++i) {
        ASSERT_EQ(v[i], it[i]);
        ASSERT_EQ(it[i], i);
    }
}

TEST(VectorIteratorTest, IteratorPlusEqual) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    auto it = v.begin();

    ASSERT_EQ(*(it += 2), 3);
}

TEST(VectorIteratorTest, IteratorMinusEqual) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    auto it = v.end();

    ASSERT_EQ(*(it -= 5), 1);
}
