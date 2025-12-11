#include "iterators.hpp"
#include "vector.hpp"
#include <gtest/gtest.h>
#include <numeric>

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

// Mixed Iterator/Const Iterator Comparison Tests

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

// Ordering Tests

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

// Algorithm Compatibility Tests

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

TEST(VectorIteratorTest, Accumulate) {
    ptorpis::vector<int> v{2, 3, 5};

    int sum = std::accumulate(v.begin(), v.end(), 0);

    EXPECT_EQ(sum, 10);
}

// Reverse Iterator Tests

TEST(VectorReverseIteratorTest, BasicReverseIteration) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    std::vector<int> result;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result.size(), 5);
    EXPECT_EQ(result[0], 5);
    EXPECT_EQ(result[1], 4);
    EXPECT_EQ(result[2], 3);
    EXPECT_EQ(result[3], 2);
    EXPECT_EQ(result[4], 1);
}

TEST(VectorReverseIteratorTest, ReverseIteratorModify) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        *it *= 2;
    }

    EXPECT_EQ(v[0], 2);  // 1 * 2
    EXPECT_EQ(v[1], 4);  // 2 * 2
    EXPECT_EQ(v[2], 6);  // 3 * 2
    EXPECT_EQ(v[3], 8);  // 4 * 2
    EXPECT_EQ(v[4], 10); // 5 * 2
}

TEST(VectorReverseIteratorTest, ReverseIteratorDereference) {
    ptorpis::vector<int> v{10, 20, 30, 40, 50};

    auto it = v.rbegin();

    EXPECT_EQ(*it, 50);
    ++it;
    EXPECT_EQ(*it, 40);
    ++it;
    EXPECT_EQ(*it, 30);
}

TEST(VectorReverseIteratorTest, ReverseIteratorComparison) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto rit1 = v.rbegin();
    auto rit2 = v.rbegin();
    auto rit3 = v.rend();

    EXPECT_TRUE(rit1 == rit2);
    EXPECT_FALSE(rit1 == rit3);
    EXPECT_TRUE(rit1 != rit3);
}

TEST(VectorReverseIteratorTest, EmptyVectorReverseIterator) {
    ptorpis::vector<int> v;

    EXPECT_TRUE(v.rbegin() == v.rend());

    int count = 0;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        ++count;
    }

    EXPECT_EQ(count, 0);
}

TEST(VectorReverseIteratorTest, SingleElementReverseIterator) {
    ptorpis::vector<int> v{42};

    auto it = v.rbegin();
    EXPECT_EQ(*it, 42);

    ++it;
    EXPECT_TRUE(it == v.rend());
}

TEST(VectorReverseIteratorTest, ReverseIteratorArithmetic) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = v.rbegin();
    it += 2;

    EXPECT_EQ(*it, 3); // rbegin points to 5, +2 goes to 3
}

// Const Reverse Iterator Tests

TEST(VectorConstReverseIteratorTest, BasicConstReverseIteration) {
    const ptorpis::vector<int> v{1, 2, 3, 4, 5};

    std::vector<int> result;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result.size(), 5);
    EXPECT_EQ(result[0], 5);
    EXPECT_EQ(result[4], 1);
}

TEST(VectorConstReverseIteratorTest, CRBeginCREnd) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    std::vector<int> result;
    for (auto it = v.crbegin(); it != v.crend(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result.size(), 5);
    EXPECT_EQ(result[0], 5);
    EXPECT_EQ(result[4], 1);
}

TEST(VectorConstReverseIteratorTest, ConstReverseIteratorReadOnly) {
    const ptorpis::vector<int> v{10, 20, 30};

    auto it = v.rbegin();
    EXPECT_EQ(*it, 30);

    // This should not compile (uncomment to verify):
    // *it = 99;
}

TEST(VectorConstReverseIteratorTest, EmptyConstReverseIterator) {
    const ptorpis::vector<int> v;

    EXPECT_TRUE(v.rbegin() == v.rend());
}

// Reverse Iterator with Algorithms

TEST(VectorReverseIteratorTest, ReverseIteratorWithAccumulate) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    int sum = std::accumulate(v.rbegin(), v.rend(), 0);

    EXPECT_EQ(sum, 15);
}

TEST(VectorReverseIteratorTest, ReverseIteratorWithFind) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto it = std::find(v.rbegin(), v.rend(), 3);

    EXPECT_NE(it, v.rend());
    EXPECT_EQ(*it, 3);
}

TEST(VectorReverseIteratorTest, ReverseIteratorWithCopy) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};
    std::vector<int> result(5);

    std::copy(v.rbegin(), v.rend(), result.begin());

    EXPECT_EQ(result[0], 5);
    EXPECT_EQ(result[4], 1);
}

// Forward and Reverse Iterator Interaction

TEST(VectorIteratorTest, ForwardAndReverseIteratorConsistency) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    // Collect forward
    std::vector<int> forward;
    for (auto it = v.begin(); it != v.end(); ++it) {
        forward.push_back(*it);
    }

    // Collect reverse
    std::vector<int> reverse;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        reverse.push_back(*it);
    }

    // Should be reverse of each other
    EXPECT_EQ(forward.size(), reverse.size());
    for (size_t i = 0; i < forward.size(); ++i) {
        EXPECT_EQ(forward[i], reverse[reverse.size() - 1 - i]);
    }
}

TEST(VectorIteratorTest, BaseIteratorConversion) {
    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    auto rit = v.rbegin();
    ++rit; // Points to 4 in reverse (index 3 in forward)

    // Convert reverse iterator to base (forward) iterator
    auto it = rit.base();

    // base() returns the forward iterator one past the element
    // rbegin+1 points to v[3] (value 4)
    // base() returns iterator to v[4] (value 5)
    EXPECT_EQ(*it, 5);
}

// Type Traits Tests

TEST(VectorReverseIteratorTest, ReverseIteratorTypeTraits) {
    using reverse_iter = ptorpis::vector<int>::reverse_iterator;
    using const_reverse_iter = ptorpis::vector<int>::const_reverse_iterator;

    static_assert(
        std::is_same_v<typename std::iterator_traits<reverse_iter>::iterator_category,
                       std::random_access_iterator_tag>);

    static_assert(std::is_same_v<
                  typename std::iterator_traits<const_reverse_iter>::iterator_category,
                  std::random_access_iterator_tag>);
}
