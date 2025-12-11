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

// Iterator Comparison Tests

TEST(VectorIteratorTest, EqualityComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin();
    auto it3 = v.begin() + 1;

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);
}

TEST(VectorIteratorTest, InequalityComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin();
    auto it3 = v.begin() + 1;

    EXPECT_FALSE(it1 != it2);
    EXPECT_TRUE(it1 != it3);
}

TEST(VectorIteratorTest, LessThanComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    auto it3 = v.end();

    EXPECT_TRUE(it1 < it2);
    EXPECT_TRUE(it2 < it3);
    EXPECT_FALSE(it2 < it1);
    EXPECT_FALSE(it1 < it1);
}

TEST(VectorIteratorTest, LessThanOrEqualComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    auto it3 = v.begin();

    EXPECT_TRUE(it1 <= it2);
    EXPECT_TRUE(it1 <= it3);
    EXPECT_FALSE(it2 <= it1);
}

TEST(VectorIteratorTest, GreaterThanComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    auto it3 = v.end();

    EXPECT_TRUE(it2 > it1);
    EXPECT_TRUE(it3 > it2);
    EXPECT_FALSE(it1 > it2);
    EXPECT_FALSE(it1 > it1);
}

TEST(VectorIteratorTest, GreaterThanOrEqualComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    auto it3 = v.begin();

    EXPECT_TRUE(it2 >= it1);
    EXPECT_TRUE(it1 >= it3);
    EXPECT_FALSE(it1 >= it2);
}

TEST(VectorIteratorTest, ComparisonWithSelf) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.begin();

    EXPECT_TRUE(it == it);
    EXPECT_FALSE(it != it);
    EXPECT_FALSE(it < it);
    EXPECT_TRUE(it <= it);
    EXPECT_FALSE(it > it);
    EXPECT_TRUE(it >= it);
}

TEST(VectorIteratorTest, BeginEndComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto begin = v.begin();
    auto end = v.end();

    EXPECT_TRUE(begin < end);
    EXPECT_TRUE(begin <= end);
    EXPECT_TRUE(end > begin);
    EXPECT_TRUE(end >= begin);
    EXPECT_FALSE(begin == end);
    EXPECT_TRUE(begin != end);
}

TEST(VectorIteratorTest, EmptyVectorIterators) {
    ptorpis::vector<int> v;

    auto begin = v.begin();
    auto end = v.end();

    EXPECT_TRUE(begin == end);
    EXPECT_FALSE(begin != end);
    EXPECT_FALSE(begin < end);
    EXPECT_TRUE(begin <= end);
    EXPECT_FALSE(begin > end);
    EXPECT_TRUE(begin >= end);
}

TEST(VectorIteratorTest, SingleElementComparison) {
    ptorpis::vector<int> v{42};

    auto begin = v.begin();
    auto end = v.end();
    auto mid = begin;

    EXPECT_TRUE(begin == mid);
    EXPECT_TRUE(begin < end);
    EXPECT_TRUE(mid < end);
}

TEST(VectorIteratorTest, MultipleIncrementComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin();

    ++it2;
    EXPECT_TRUE(it1 < it2);

    ++it2;
    EXPECT_TRUE(it1 < it2);

    ++it1;
    ++it1;
    EXPECT_TRUE(it1 == it2);
}

// Const Iterator Comparison Tests

TEST(VectorConstIteratorTest, EqualityComparison) {
    const ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.cbegin();
    auto it2 = v.cbegin();
    auto it3 = v.cbegin() + 1;

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);
}

TEST(VectorConstIteratorTest, InequalityComparison) {
    const ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.cbegin();
    auto it2 = v.cbegin();
    auto it3 = v.cbegin() + 1;

    EXPECT_FALSE(it1 != it2);
    EXPECT_TRUE(it1 != it3);
}

TEST(VectorConstIteratorTest, LessThanComparison) {
    const ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    auto it3 = v.end();

    EXPECT_TRUE(it1 < it2);
    EXPECT_TRUE(it2 < it3);
    EXPECT_FALSE(it2 < it1);
    EXPECT_FALSE(it1 < it1);
}

TEST(VectorConstIteratorTest, LessThanOrEqualComparison) {
    const ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    auto it3 = v.begin();

    EXPECT_TRUE(it1 <= it2);
    EXPECT_TRUE(it1 <= it3);
    EXPECT_FALSE(it2 <= it1);
}

TEST(VectorConstIteratorTest, GreaterThanComparison) {
    const ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    auto it3 = v.end();

    EXPECT_TRUE(it2 > it1);
    EXPECT_TRUE(it3 > it2);
    EXPECT_FALSE(it1 > it2);
    EXPECT_FALSE(it1 > it1);
}

TEST(VectorConstIteratorTest, GreaterThanOrEqualComparison) {
    const ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    auto it3 = v.begin();

    EXPECT_TRUE(it2 >= it1);
    EXPECT_TRUE(it1 >= it3);
    EXPECT_FALSE(it1 >= it2);
}

TEST(VectorConstIteratorTest, ComparisonWithSelf) {
    const ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.begin();

    EXPECT_TRUE(it == it);
    EXPECT_FALSE(it != it);
    EXPECT_FALSE(it < it);
    EXPECT_TRUE(it <= it);
    EXPECT_FALSE(it > it);
    EXPECT_TRUE(it >= it);
}

TEST(VectorConstIteratorTest, BeginEndComparison) {
    const ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto begin = v.begin();
    auto end = v.end();

    EXPECT_TRUE(begin < end);
    EXPECT_TRUE(begin <= end);
    EXPECT_TRUE(end > begin);
    EXPECT_TRUE(end >= begin);
    EXPECT_FALSE(begin == end);
    EXPECT_TRUE(begin != end);
}

TEST(VectorConstIteratorTest, EmptyVectorIterators) {
    const ptorpis::vector<int> v;

    auto begin = v.begin();
    auto end = v.end();

    EXPECT_TRUE(begin == end);
    EXPECT_FALSE(begin != end);
    EXPECT_FALSE(begin < end);
    EXPECT_TRUE(begin <= end);
    EXPECT_FALSE(begin > end);
    EXPECT_TRUE(begin >= end);
}

TEST(VectorConstIteratorTest, CBeginCEndComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto begin = v.cbegin();
    auto end = v.cend();
    auto mid = v.cbegin() + 2;

    EXPECT_TRUE(begin < mid);
    EXPECT_TRUE(mid < end);
    EXPECT_TRUE(begin < end);
}

// ============================================================================
// Mixed Iterator/Const Iterator Comparison Tests
// ============================================================================

TEST(VectorMixedIteratorTest, IteratorToConstIteratorComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.begin();
    auto cit = v.cbegin();

    // Should be able to compare iterator with const_iterator
    EXPECT_TRUE(it == cit);
    EXPECT_FALSE(it != cit);
}

TEST(VectorMixedIteratorTest, IteratorAndConstBeginEnd) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.begin();
    auto cend = v.cend();

    EXPECT_TRUE(it < cend);
    EXPECT_TRUE(cend > it);
}

// ============================================================================
// Ordering Tests
// ============================================================================

TEST(VectorIteratorTest, TransitiveOrdering) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;
    auto it3 = v.begin() + 4;

    // If it1 < it2 and it2 < it3, then it1 < it3
    EXPECT_TRUE(it1 < it2);
    EXPECT_TRUE(it2 < it3);
    EXPECT_TRUE(it1 < it3);
}

TEST(VectorIteratorTest, AntiSymmetricOrdering) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;

    // If it1 < it2, then !(it2 < it1)
    EXPECT_TRUE(it1 < it2);
    EXPECT_FALSE(it2 < it1);
}

TEST(VectorIteratorTest, ComplementaryComparisons) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it1 = v.begin();
    auto it2 = v.begin() + 2;

    // (it1 < it2) is equivalent to (it2 > it1)
    EXPECT_EQ(it1<it2, it2> it1);

    // (it1 <= it2) is equivalent to (it2 >= it1)
    EXPECT_EQ(it1 <= it2, it2 >= it1);
}

// ============================================================================
// Algorithm Compatibility Tests
// ============================================================================

TEST(VectorIteratorTest, SortRequiresComparison) {
    ptorpis::vector<int> v{5, 2, 8, 1, 9, 3};

    std::sort(v.begin(), v.end()); // Requires < operator

    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[5], 9);
}

TEST(VectorIteratorTest, BinarySearchRequiresComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};

    bool found = std::binary_search(v.begin(), v.end(), 5); // Requires < operator

    EXPECT_TRUE(found);
}

TEST(VectorIteratorTest, MinMaxElementRequiresComparison) {
    ptorpis::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};

    auto min_it = std::min_element(v.begin(), v.end()); // Requires < operator
    auto max_it = std::max_element(v.begin(), v.end());

    EXPECT_EQ(*min_it, 1);
    EXPECT_EQ(*max_it, 9);
}
