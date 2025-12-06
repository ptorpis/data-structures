#include "vector.hpp"
#include <gtest/gtest.h>
#include <string>

TEST(VectorTypeTest, StringVector) {
    ptorpis::vector<std::string> v{"hello", "world"};

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], "hello");
    EXPECT_EQ(v[1], "world");
}

TEST(VectorTypeTest, StringCopyConstructor) {
    ptorpis::vector<std::string> v1{"hello", "world"};
    ptorpis::vector<std::string> v2(v1);

    EXPECT_EQ(v2.size(), 2);
    EXPECT_EQ(v2[0], "hello");
    EXPECT_EQ(v2[1], "world");

    // Deep copy check
    v1[0] = "goodbye";
    EXPECT_EQ(v2[0], "hello");
}

TEST(VectorTypeTest, DoubleVector) {
    ptorpis::vector<double> v{1.1, 2.2, 3.3};

    EXPECT_EQ(v.size(), 3);
    EXPECT_DOUBLE_EQ(v[0], 1.1);
    EXPECT_DOUBLE_EQ(v[1], 2.2);
    EXPECT_DOUBLE_EQ(v[2], 3.3);
}
