#include "vector.hpp"
#include <gtest/gtest.h>

TEST(VectorOperatorTest, EqualityOperator) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2{1, 2, 3};
    ptorpis::vector<int> v3{1, 2, 4};
    ptorpis::vector<int> v4{1, 2};

    EXPECT_TRUE(v1 == v2);  // Same contents
    EXPECT_FALSE(v1 == v3); // Different element
    EXPECT_FALSE(v1 == v4); // Different size
}

TEST(VectorOperatorTest, InequalityOperator) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2{1, 2, 3};
    ptorpis::vector<int> v3{4, 5, 6};

    EXPECT_FALSE(v1 != v2); // Same contents
    EXPECT_TRUE(v1 != v3);  // Different contents
}

TEST(VectorOperatorTest, EqualityEmptyVectors) {
    ptorpis::vector<int> v1;
    ptorpis::vector<int> v2;

    EXPECT_TRUE(v1 == v2); // Both empty
}

TEST(VectorOperatorTest, EqualitySelfComparison) {
    ptorpis::vector<int> v{1, 2, 3};

    EXPECT_TRUE(v == v); // Should equal itself
}
