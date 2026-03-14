#include "forward_list.hpp"
#include "lifetime.hpp"
#include <gtest/gtest.h>
#include <vector>

static std::vector<int> to_vec(const ptorpis::forward_list<int>& list) {
    std::vector<int> out;
    for (const auto v : list) out.push_back(v);
    return out;
}

// ── Basic functionality ───────────────────────────────────────────────────────

TEST(ForwardList, StartsEmpty) {
    ptorpis::forward_list<int> list{};
    EXPECT_TRUE(list.empty());
}

TEST(ForwardList, NotEmptyAfterPushFront) {
    ptorpis::forward_list<int> list{};
    list.push_front(1);
    EXPECT_FALSE(list.empty());
}

TEST(ForwardList, PushFrontOrder) {
    ptorpis::forward_list<int> list{};
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    std::vector<int> result = to_vec(list);
    EXPECT_EQ(result, (std::vector<int>{3, 2, 1}));
}

TEST(ForwardList, PopFront) {
    ptorpis::forward_list<int> list{};
    list.push_front(1);
    list.push_front(2);
    list.pop_front();

    std::vector<int> result = to_vec(list);
    EXPECT_EQ(result, (std::vector<int>{1}));
}

TEST(ForwardList, PopFrontUntilEmpty) {
    ptorpis::forward_list<int> list{};
    list.push_front(1);
    list.push_front(2);
    list.pop_front();
    list.pop_front();
    EXPECT_TRUE(list.empty());
}

TEST(ForwardList, EmplaceFrontReturnsRef) {
    ptorpis::forward_list<int> list{};
    int& ref = list.emplace_front(99);
    EXPECT_EQ(ref, 99);
}

TEST(ForwardList, EmplaceFrontOrder) {
    ptorpis::forward_list<int> list{};
    list.emplace_front(1);
    list.emplace_front(2);
    list.emplace_front(3);

    std::vector<int> result = to_vec(list);
    EXPECT_EQ(result, (std::vector<int>{3, 2, 1}));
}

TEST(ForwardList, RangeBasedFor) {
    ptorpis::forward_list<int> list{};
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    int sum = 0;
    for (const auto v : list) sum += v;
    EXPECT_EQ(sum, 6);
}

class LifetimeCounterTest : public ::testing::Test {
protected:
    void SetUp() override { Lifetime::counters.reset(); }
};

TEST_F(LifetimeCounterTest, PushFrontLvalueCopies) {
    ptorpis::forward_list<Lifetime> list{};
    Lifetime obj{1};
    Lifetime::counters.reset(); // exclude the construction of obj

    list.push_front(obj);

    EXPECT_EQ(Lifetime::counters.copy_construct, 1);
    EXPECT_EQ(Lifetime::counters.move_construct, 0);
}

TEST_F(LifetimeCounterTest, PushFrontRvalueMoves) {
    ptorpis::forward_list<Lifetime> list{};

    list.push_front(Lifetime{1});

    EXPECT_EQ(Lifetime::counters.move_construct, 1);
    EXPECT_EQ(Lifetime::counters.copy_construct, 0);
}

TEST_F(LifetimeCounterTest, EmplaceFrontConstructsInPlace) {
    ptorpis::forward_list<Lifetime> list{};

    list.emplace_front(42);

    // Only one constructor call, no copies or moves
    EXPECT_EQ(Lifetime::counters.constructor, 1);
    EXPECT_EQ(Lifetime::counters.copy_construct, 0);
    EXPECT_EQ(Lifetime::counters.move_construct, 0);
}

TEST_F(LifetimeCounterTest, PopFrontDestructs) {
    ptorpis::forward_list<Lifetime> list{};
    list.emplace_front(1);
    list.emplace_front(2);
    Lifetime::counters.reset();

    list.pop_front();

    EXPECT_EQ(Lifetime::counters.destructor, 1);
}

TEST_F(LifetimeCounterTest, IterationDoesNotCopyOrMove) {
    ptorpis::forward_list<Lifetime> list{};
    list.emplace_front(1);
    list.emplace_front(2);
    list.emplace_front(3);
    Lifetime::counters.reset();

    for (const auto& elem : list) {
        (void)elem;
    }

    EXPECT_EQ(Lifetime::counters.copy_construct, 0);
    EXPECT_EQ(Lifetime::counters.move_construct, 0);
    EXPECT_EQ(Lifetime::counters.copy_assign, 0);
    EXPECT_EQ(Lifetime::counters.move_assign, 0);
}
