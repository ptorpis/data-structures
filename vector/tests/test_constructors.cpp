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

TEST(VectorConstructorTest, MoveConstructor) {
    ptorpis::vector<int> v1{1, 2, 3, 4, 5};
    int* old_ptr = v1.data();
    size_t old_size = v1.size();
    size_t old_cap = v1.capacity();

    ptorpis::vector<int> v2(std::move(v1));

    EXPECT_EQ(v2.data(), old_ptr);
    EXPECT_EQ(v2.size(), old_size);
    EXPECT_EQ(v2.capacity(), old_cap);
    EXPECT_EQ(v2[0], 1);

    EXPECT_EQ(v1.data(), nullptr);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);
}

TEST(VectorConstructorTest, MoveConstructorNoexcept) {
    EXPECT_TRUE(std::is_nothrow_move_constructible_v<ptorpis::vector<int>>);
}

TEST(VectorCopyAssignmentTest, BasicCopyAssignment) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2{4, 5, 6, 7, 8};

    v1 = v2;

    EXPECT_EQ(v1.size(), 5);
    EXPECT_EQ(v1[0], 4);
    EXPECT_EQ(v1[4], 8);

    EXPECT_NE(v1.data(), v2.data());
}

TEST(VectorCopyAssignmentTest, SelfAssignment) {
    ptorpis::vector<int> v{1, 2, 3};
    ptorpis::vector<int>& ref = v;

    v = ref; // Self-assignment via reference - no warning

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
}
TEST(VectorCopyAssignmentTest, AssignEmpty) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2;

    v1 = v2;

    EXPECT_EQ(v1.size(), 0);
    EXPECT_TRUE(v1.empty());
}

TEST(VectorCopyAssignmentTest, AssignToEmpty) {
    ptorpis::vector<int> v1;
    ptorpis::vector<int> v2{1, 2, 3};

    v1 = v2;

    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v1[2], 3);
}

TEST(VectorCopyAssignmentTest, AssignDifferentSizes) {
    ptorpis::vector<int> v1{1, 2};
    ptorpis::vector<int> v2{3, 4, 5, 6, 7, 8};

    v1 = v2;

    EXPECT_EQ(v1.size(), 6);
    for (std::size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

TEST(VectorCopyAssignmentTest, IndependentAfterAssignment) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2{4, 5};

    v1 = v2;

    // Modify v2
    v2[0] = 999;

    // v1 should be unchanged
    EXPECT_EQ(v1[0], 4);
    EXPECT_EQ(v2[0], 999);
}

TEST(VectorCopyAssignmentTest, AssignStrings) {
    ptorpis::vector<std::string> v1{"a", "b"};
    ptorpis::vector<std::string> v2{"hello", "world", "test"};

    v1 = v2;

    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1[0], "hello");
    EXPECT_EQ(v1[2], "test");
}

TEST(VectorCopyAssignmentTest, Chaining) {
    ptorpis::vector<int> v1{1, 2};
    ptorpis::vector<int> v2{3, 4};
    ptorpis::vector<int> v3{5, 6};

    v1 = v2 = v3;

    EXPECT_EQ(v1.size(), 2);
    EXPECT_EQ(v1[0], 5);
    EXPECT_EQ(v2[0], 5);
    EXPECT_EQ(v3[0], 5);
}

TEST(VectorMoveAssignmentTest, SelfMoveAssignment) {
    ptorpis::vector<int> v{1, 2, 3};
    ptorpis::vector<int>& ref = v;

    v = std::move(ref); // Self-move via reference - no warning

    EXPECT_NO_THROW(v.size());
}

TEST(VectorMoveAssignmentTest, BasicMoveAssignment) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2{4, 5, 6, 7, 8};

    int* old_ptr = v2.data();
    std::size_t old_size = v2.size();

    v1 = std::move(v2);

    EXPECT_EQ(v1.data(), old_ptr);
    EXPECT_EQ(v1.size(), old_size);
    EXPECT_EQ(v1[0], 4);
    EXPECT_EQ(v1[4], 8);

    EXPECT_EQ(v2.size(), 0);
    EXPECT_EQ(v2.data(), nullptr);
}

TEST(VectorMoveAssignmentTest, MoveAssignEmpty) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2;

    v1 = std::move(v2);

    EXPECT_EQ(v1.size(), 0);
    EXPECT_TRUE(v1.empty());
}

TEST(VectorMoveAssignmentTest, MoveAssignToEmpty) {
    ptorpis::vector<int> v1;
    ptorpis::vector<int> v2{1, 2, 3};

    int* old_ptr = v2.data();

    v1 = std::move(v2);

    EXPECT_EQ(v1.data(), old_ptr);
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v2.size(), 0);
}

TEST(VectorMoveAssignmentTest, MoveAssignStrings) {
    ptorpis::vector<std::string> v1{"a", "b"};
    ptorpis::vector<std::string> v2{"hello", "world", "test"};

    v1 = std::move(v2);

    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1[0], "hello");
    EXPECT_EQ(v2.size(), 0);
}

TEST(VectorMoveAssignmentTest, MovedFromVectorIsUsable) {
    ptorpis::vector<int> v1{1, 2, 3};
    ptorpis::vector<int> v2{4, 5, 6};

    v1 = std::move(v2);

    // v2 should still be usable
    v2 = ptorpis::vector<int>{7, 8, 9};
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2[0], 7);
}

TEST(VectorMoveAssignmentTest, NoexceptGuarantee) {
    EXPECT_TRUE(std::is_nothrow_move_assignable_v<ptorpis::vector<int>>);
    EXPECT_TRUE(std::is_nothrow_move_assignable_v<ptorpis::vector<std::string>>);
}

TEST(VectorMoveAssignmentTest, OldResourcesDestroyed) {
    // This test relies on AddressSanitizer or Valgrind to detect leaks
    ptorpis::vector<std::string> v1{"old1", "old2", "old3"};
    ptorpis::vector<std::string> v2{"new1", "new2"};

    v1 = std::move(v2); // Old strings in v1 should be destroyed

    EXPECT_EQ(v1.size(), 2);
    EXPECT_EQ(v1[0], "new1");
}

TEST(VectorDestructorTest, DestructorCalledOnScopeExit) {
    // This test mainly relies on AddressSanitizer to detect leaks
    {
        ptorpis::vector<int> v{1, 2, 3, 4, 5};
        // Destructor called here
    }
    // If there's a leak, ASan will catch it
}

TEST(VectorDestructorTest, DestructorWithStrings) {
    // Ensures string destructors are called
    {
        ptorpis::vector<std::string> v{"hello", "world", "test"};
        // String destructors should be called
    }
    // No memory leaks should occur
}

TEST(VectorDestructorTest, DestructorOnEmpty) {
    {
        ptorpis::vector<int> v;
        // Should safely destruct empty vector
    }
}

TEST(VectorDestructorTest, DestructorAfterMove) {
    ptorpis::vector<int> v1{1, 2, 3};
    {
        ptorpis::vector<int> v2(std::move(v1));
        // v2 destructor runs here
    }
    // v1 destructor runs here (should be safe on empty vector)
}

/*
 * Type trait Tests
 */
TEST(VectorTypeTraitsTest, CopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<ptorpis::vector<int>>);
    EXPECT_TRUE(std::is_copy_constructible_v<ptorpis::vector<std::string>>);
}

TEST(VectorTypeTraitsTest, MoveConstructible) {
    EXPECT_TRUE(std::is_move_constructible_v<ptorpis::vector<int>>);
    EXPECT_TRUE(std::is_move_constructible_v<ptorpis::vector<std::string>>);
}

TEST(VectorTypeTraitsTest, CopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<ptorpis::vector<int>>);
    EXPECT_TRUE(std::is_copy_assignable_v<ptorpis::vector<std::string>>);
}

TEST(VectorTypeTraitsTest, MoveAssignable) {
    EXPECT_TRUE(std::is_move_assignable_v<ptorpis::vector<int>>);
    EXPECT_TRUE(std::is_move_assignable_v<ptorpis::vector<std::string>>);
}

TEST(VectorTypeTraitsTest, Destructible) {
    EXPECT_TRUE(std::is_destructible_v<ptorpis::vector<int>>);
    EXPECT_TRUE(std::is_destructible_v<ptorpis::vector<std::string>>);
}

TEST(VectorTypeTraitsTest, NothrowMoveConstructible) {
    EXPECT_TRUE(std::is_nothrow_move_constructible_v<ptorpis::vector<int>>);
}

TEST(VectorTypeTraitsTest, NothrowMoveAssignable) {
    EXPECT_TRUE(std::is_nothrow_move_assignable_v<ptorpis::vector<int>>);
}
