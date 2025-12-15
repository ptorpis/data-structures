#include "vector.hpp"
#include <gtest/gtest.h>
#include <list>
#include <stdexcept>
#include <vector>

// ThrowAfterN that can throw an exception after N constructions
class ThrowAfterN {
private:
    int value_;
    static int construction_count_;
    static int throw_after_;
    static bool throw_enabled_;

public:
    static void reset(int throw_after = -1) {
        construction_count_ = 0;
        throw_after_ = throw_after;
        throw_enabled_ = (throw_after >= 0);
    }

    static int getConstructionCount() { return construction_count_; }

    ThrowAfterN() : value_(0) {
        if (throw_enabled_ && construction_count_ >= throw_after_) {
            throw std::runtime_error("Construction limit reached");
        }
        ++construction_count_;
    }

    explicit ThrowAfterN(int val) : value_(val) {
        if (throw_enabled_ && construction_count_ >= throw_after_) {
            throw std::runtime_error("Construction limit reached");
        }
        ++construction_count_;
    }

    ThrowAfterN(const ThrowAfterN& other) : value_(other.value_) {
        if (throw_enabled_ && construction_count_ >= throw_after_) {
            throw std::runtime_error("Construction limit reached");
        }
        ++construction_count_;
    }

    ThrowAfterN(ThrowAfterN&& other) noexcept : value_(other.value_) { other.value_ = 0; }

    ThrowAfterN& operator=(const ThrowAfterN& other) {
        if (this != &other) {
            value_ = other.value_;
        }
        return *this;
    }

    ThrowAfterN& operator=(ThrowAfterN&& other) noexcept {
        if (this != &other) {
            value_ = other.value_;
            other.value_ = 0;
        }
        return *this;
    }

    int getValue() const { return value_; }

    bool operator==(const ThrowAfterN& other) const { return value_ == other.value_; }
};

int ThrowAfterN::construction_count_ = 0;
int ThrowAfterN::throw_after_ = -1;
bool ThrowAfterN::throw_enabled_ = false;

class VectorExceptionSafetyTest : public ::testing::Test {
protected:
    void SetUp() override {
        ThrowAfterN::reset(); // Disable throwing before each test
    }

    void TearDown() override {
        ThrowAfterN::reset(); // Clean up after each test
    }

    // Helper function to create a reference vector with correct values
    std::vector<ThrowAfterN> createReferenceVector(const std::vector<int>& values) {
        ThrowAfterN::reset(); // Make sure we can construct without throwing
        std::vector<ThrowAfterN> result;
        for (int val : values) {
            result.push_back(ThrowAfterN(val));
        }
        return result;
    }

    // Helper to verify vector contents match expected values
    template <typename Vector>
    void verifyContents(const Vector& vec, const std::vector<int>& expected) {
        ASSERT_EQ(vec.size(), expected.size()) << "Size mismatch";
        for (size_t i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(vec[i].getValue(), expected[i]) << "Value mismatch at index " << i;
        }
    }
};

TEST_F(VectorExceptionSafetyTest, InsertLValueAtBeginning_NoReallocation) {
    ptorpis::vector<ThrowAfterN> vec;

    // Pre-populate with capacity to avoid reallocation
    ThrowAfterN::reset();
    vec.reserve(10);
    vec.push_back(ThrowAfterN(1));
    vec.push_back(ThrowAfterN(2));
    vec.push_back(ThrowAfterN(3));

    std::vector<int> original = {1, 2, 3};

    // Try throwing at different construction points
    for (int throw_at = 1; throw_at < 5; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        test_vec.reserve(10);
        ThrowAfterN::reset();
        test_vec.push_back(ThrowAfterN(1));
        test_vec.push_back(ThrowAfterN(2));
        test_vec.push_back(ThrowAfterN(3));

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        ThrowAfterN value(99); // first construction, this is why the loop starts at 1
        ThrowAfterN::reset(throw_at); // Reset after creating value

        try {
            test_vec.insert(test_vec.begin(), value);
            // If we get here, no exception was thrown
        } catch (const std::runtime_error&) {
            // Strong guarantee: verify vector is unchanged
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertLValueAtEnd_NoReallocation) {
    ptorpis::vector<ThrowAfterN> vec;

    ThrowAfterN::reset();
    vec.reserve(10);
    vec.push_back(ThrowAfterN(1));
    vec.push_back(ThrowAfterN(2));
    vec.push_back(ThrowAfterN(3));

    std::vector<int> original = {1, 2, 3};

    for (int throw_at = 1; throw_at < 5; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        test_vec.reserve(10);
        ThrowAfterN::reset();
        test_vec.push_back(ThrowAfterN(1));
        test_vec.push_back(ThrowAfterN(2));
        test_vec.push_back(ThrowAfterN(3));

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        ThrowAfterN value(99);
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.end(), value);
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertLValueInMiddle_NoReallocation) {
    ptorpis::vector<ThrowAfterN> vec;

    ThrowAfterN::reset();
    vec.reserve(10);
    vec.push_back(ThrowAfterN(1));
    vec.push_back(ThrowAfterN(2));
    vec.push_back(ThrowAfterN(3));
    vec.push_back(ThrowAfterN(4));

    std::vector<int> original = {1, 2, 3, 4};

    for (int throw_at = 1; throw_at < 5; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        test_vec.reserve(10);
        ThrowAfterN::reset();
        test_vec.push_back(ThrowAfterN(1));
        test_vec.push_back(ThrowAfterN(2));
        test_vec.push_back(ThrowAfterN(3));
        test_vec.push_back(ThrowAfterN(4));

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        ThrowAfterN value(99);
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.begin() + 2, value);
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertLValue_WithReallocation) {
    ptorpis::vector<ThrowAfterN> vec;

    ThrowAfterN::reset();
    vec.push_back(ThrowAfterN(1));
    vec.push_back(ThrowAfterN(2));
    vec.push_back(ThrowAfterN(3));

    std::vector<int> original = {1, 2, 3};

    // Force reallocation by not reserving extra space
    for (int throw_at = 1; throw_at < 10; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        ThrowAfterN::reset();
        test_vec.push_back(ThrowAfterN(1));
        test_vec.push_back(ThrowAfterN(2));
        test_vec.push_back(ThrowAfterN(3));

        // Ensure it's at capacity to force reallocation
        test_vec.shrink_to_fit(); // If you have this

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        ThrowAfterN value(99);
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.begin() + 1, value);
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertRValue_NoReallocation) {
    ptorpis::vector<ThrowAfterN> vec;

    ThrowAfterN::reset();
    vec.reserve(10);
    vec.push_back(ThrowAfterN(1));
    vec.push_back(ThrowAfterN(2));
    vec.push_back(ThrowAfterN(3));

    std::vector<int> original = {1, 2, 3};

    for (int throw_at = 1; throw_at < 5; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        test_vec.reserve(10);
        ThrowAfterN::reset();
        test_vec.push_back(ThrowAfterN(1));
        test_vec.push_back(ThrowAfterN(2));
        test_vec.push_back(ThrowAfterN(3));

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.begin() + 1, ThrowAfterN(99));
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertMultiple_NoReallocation) {
    ptorpis::vector<ThrowAfterN> vec;

    ThrowAfterN::reset();
    vec.reserve(20);
    vec.push_back(ThrowAfterN(1));
    vec.push_back(ThrowAfterN(2));
    vec.push_back(ThrowAfterN(3));

    std::vector<int> original = {1, 2, 3};

    for (int throw_at = 1; throw_at < 10; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        test_vec.reserve(20);
        ThrowAfterN::reset();
        test_vec.push_back(ThrowAfterN(1));
        test_vec.push_back(ThrowAfterN(2));
        test_vec.push_back(ThrowAfterN(3));

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        ThrowAfterN value(99);
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.begin() + 1, 5, value);
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertMultiple_WithReallocation) {
    ptorpis::vector<ThrowAfterN> vec;

    ThrowAfterN::reset();
    vec.push_back(ThrowAfterN(1));
    vec.push_back(ThrowAfterN(2));
    vec.push_back(ThrowAfterN(3));

    std::vector<int> original = {1, 2, 3};

    for (int throw_at = 1; throw_at < 15; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        ThrowAfterN::reset();
        test_vec.push_back(ThrowAfterN(1));
        test_vec.push_back(ThrowAfterN(2));
        test_vec.push_back(ThrowAfterN(3));

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        ThrowAfterN value(99);
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.begin() + 1, 10, value);
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertZeroElements) {
    ptorpis::vector<ThrowAfterN> vec;

    ThrowAfterN::reset();
    vec.push_back(ThrowAfterN(1));
    vec.push_back(ThrowAfterN(2));

    ThrowAfterN value(99);

    auto it = vec.insert(vec.begin() + 1, 0, value);

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0].getValue(), 1);
    EXPECT_EQ(vec[1].getValue(), 2);
    EXPECT_EQ(it, vec.begin() + 1);
}

TEST_F(VectorExceptionSafetyTest, InsertMultiple_VariousPositions) {
    std::vector<int> original = {1, 2, 3, 4, 5};

    // Test inserting at beginning, middle, and end
    std::vector<size_t> positions = {0, 2, 5};

    for (size_t pos : positions) {
        for (int throw_at = 1; throw_at < 8; ++throw_at) {
            ptorpis::vector<ThrowAfterN> test_vec;
            test_vec.reserve(20);
            ThrowAfterN::reset();

            for (int val : original) {
                test_vec.push_back(ThrowAfterN(val));
            }

            size_t original_size = test_vec.size();
            ThrowAfterN::reset(throw_at);

            ThrowAfterN value(99);
            ThrowAfterN::reset(throw_at);

            try {
                test_vec.insert(test_vec.begin() + pos, 3, value);
            } catch (const std::runtime_error&) {
                EXPECT_EQ(test_vec.size(), original_size);
                verifyContents(test_vec, original);
            }
        }
    }
}

TEST_F(VectorExceptionSafetyTest, SuccessfulInsertions) {
    ptorpis::vector<ThrowAfterN> vec;

    ThrowAfterN::reset();
    vec.push_back(ThrowAfterN(1));
    vec.push_back(ThrowAfterN(2));
    vec.push_back(ThrowAfterN(3));

    // Insert single element
    ThrowAfterN value(99);
    vec.insert(vec.begin() + 1, value);

    EXPECT_EQ(vec.size(), 4);
    verifyContents(vec, {1, 99, 2, 3});

    // Insert multiple elements
    vec.insert(vec.begin() + 2, 2, ThrowAfterN(88));

    EXPECT_EQ(vec.size(), 6);
    verifyContents(vec, {1, 99, 88, 88, 2, 3});
}

// Test Cases for insert(const_iterator pos, InputIt first, InputIt last)

TEST_F(VectorExceptionSafetyTest, InsertRange_AtBeginning_NoReallocation) {
    ptorpis::vector<int> vec;
    vec.reserve(10);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::vector<int> to_insert = {10, 20, 30};
    vec.insert(vec.begin(), to_insert.begin(), to_insert.end());

    EXPECT_EQ(vec.size(), 6);
    std::vector<int> expected = {10, 20, 30, 1, 2, 3};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

TEST_F(VectorExceptionSafetyTest, InsertRange_AtEnd_NoReallocation) {
    ptorpis::vector<int> vec;
    vec.reserve(10);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::vector<int> to_insert = {10, 20, 30};
    vec.insert(vec.end(), to_insert.begin(), to_insert.end());

    EXPECT_EQ(vec.size(), 6);
    std::vector<int> expected = {1, 2, 3, 10, 20, 30};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

TEST_F(VectorExceptionSafetyTest, InsertRange_InMiddle_NoReallocation) {
    ptorpis::vector<int> vec;
    vec.reserve(10);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    std::vector<int> to_insert = {10, 20};
    vec.insert(vec.begin() + 2, to_insert.begin(), to_insert.end());

    EXPECT_EQ(vec.size(), 6);
    std::vector<int> expected = {1, 2, 10, 20, 3, 4};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

TEST_F(VectorExceptionSafetyTest, InsertRange_EmptyRange) {
    ptorpis::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::vector<int> to_insert;
    auto it = vec.insert(vec.begin() + 1, to_insert.begin(), to_insert.end());

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(it, vec.begin() + 1);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST_F(VectorExceptionSafetyTest, InsertRange_WithReallocation) {
    ptorpis::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::vector<int> to_insert = {10, 20, 30, 40, 50};
    vec.insert(vec.begin() + 1, to_insert.begin(), to_insert.end());

    EXPECT_EQ(vec.size(), 8);
    std::vector<int> expected = {1, 10, 20, 30, 40, 50, 2, 3};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

TEST_F(VectorExceptionSafetyTest, InsertRange_FromDifferentContainers) {
    ptorpis::vector<int> vec = {1, 2, 3};

    // From std::list
    std::list<int> from_list = {10, 20};
    vec.insert(vec.begin() + 1, from_list.begin(), from_list.end());

    EXPECT_EQ(vec.size(), 5);
    std::vector<int> expected = {1, 10, 20, 2, 3};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }

    // From C-array
    int arr[] = {100, 200};
    vec.insert(vec.end(), arr, arr + 2);

    EXPECT_EQ(vec.size(), 7);
    expected = {1, 10, 20, 2, 3, 100, 200};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

// Exception Safety Tests for insert(const_iterator, InputIt, InputIt)

TEST_F(VectorExceptionSafetyTest, InsertRange_ExceptionSafety_NoReallocation) {
    std::vector<int> original = {1, 2, 3, 4};

    for (int throw_at = 1; throw_at < 8; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        test_vec.reserve(20);
        ThrowAfterN::reset();

        for (int val : original) {
            test_vec.push_back(ThrowAfterN(val));
        }

        std::vector<ThrowAfterN> to_insert = createReferenceVector({10, 20, 30});

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.begin() + 2, to_insert.begin(), to_insert.end());
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertRange_ExceptionSafety_WithReallocation) {
    std::vector<int> original = {1, 2, 3};

    for (int throw_at = 1; throw_at < 15; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        ThrowAfterN::reset();

        for (int val : original) {
            test_vec.push_back(ThrowAfterN(val));
        }

        // Create range to insert (will force reallocation)
        std::vector<ThrowAfterN> to_insert = createReferenceVector({10, 20, 30, 40, 50});

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.begin() + 1, to_insert.begin(), to_insert.end());
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertRange_ExceptionSafety_VariousPositions) {
    std::vector<int> original = {1, 2, 3, 4, 5};
    std::vector<size_t> positions = {0, 2, 5};

    for (size_t pos : positions) {
        for (int throw_at = 1; throw_at < 10; ++throw_at) {
            ptorpis::vector<ThrowAfterN> test_vec;
            test_vec.reserve(20);
            ThrowAfterN::reset();

            for (int val : original) {
                test_vec.push_back(ThrowAfterN(val));
            }

            std::vector<ThrowAfterN> to_insert = createReferenceVector({10, 20, 30});

            size_t original_size = test_vec.size();
            ThrowAfterN::reset(throw_at);

            try {
                test_vec.insert(test_vec.begin() + pos, to_insert.begin(),
                                to_insert.end());
            } catch (const std::runtime_error&) {
                EXPECT_EQ(test_vec.size(), original_size);
                verifyContents(test_vec, original);
            }
        }
    }
}

// Test Cases for insert(const_iterator pos, std::initializer_list<T>)

TEST_F(VectorExceptionSafetyTest, InsertInitList_AtBeginning_NoReallocation) {
    ptorpis::vector<int> vec;
    vec.reserve(10);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    vec.insert(vec.begin(), {10, 20, 30});

    EXPECT_EQ(vec.size(), 6);
    std::vector<int> expected = {10, 20, 30, 1, 2, 3};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

TEST_F(VectorExceptionSafetyTest, InsertInitList_AtEnd_NoReallocation) {
    ptorpis::vector<int> vec;
    vec.reserve(10);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    vec.insert(vec.end(), {10, 20, 30});

    EXPECT_EQ(vec.size(), 6);
    std::vector<int> expected = {1, 2, 3, 10, 20, 30};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

TEST_F(VectorExceptionSafetyTest, InsertInitList_InMiddle_NoReallocation) {
    ptorpis::vector<int> vec;
    vec.reserve(10);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);

    vec.insert(vec.begin() + 2, {10, 20});

    EXPECT_EQ(vec.size(), 6);
    std::vector<int> expected = {1, 2, 10, 20, 3, 4};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

TEST_F(VectorExceptionSafetyTest, InsertInitList_Empty) {
    ptorpis::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    auto it = vec.insert(vec.begin() + 1, {});

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(it, vec.begin() + 1);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST_F(VectorExceptionSafetyTest, InsertInitList_WithReallocation) {
    ptorpis::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    vec.insert(vec.begin() + 1, {10, 20, 30, 40, 50});

    EXPECT_EQ(vec.size(), 8);
    std::vector<int> expected = {1, 10, 20, 30, 40, 50, 2, 3};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

TEST_F(VectorExceptionSafetyTest, InsertInitList_SingleElement) {
    ptorpis::vector<int> vec = {1, 2, 3};
    vec.insert(vec.begin() + 1, {99});

    EXPECT_EQ(vec.size(), 4);
    std::vector<int> expected = {1, 99, 2, 3};
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(vec[i], expected[i]);
    }
}

// Exception Safety Tests for insert(const_iterator, initializer_list)

TEST_F(VectorExceptionSafetyTest, InsertInitList_ExceptionSafety_NoReallocation) {
    std::vector<int> original = {1, 2, 3, 4};

    for (int throw_at = 1; throw_at < 8; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        test_vec.reserve(20);
        ThrowAfterN::reset();

        for (int val : original) {
            test_vec.push_back(ThrowAfterN(val));
        }

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.begin() + 2,
                            {ThrowAfterN(10), ThrowAfterN(20), ThrowAfterN(30)});
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertInitList_ExceptionSafety_WithReallocation) {
    std::vector<int> original = {1, 2, 3};

    for (int throw_at = 1; throw_at < 15; ++throw_at) {
        ptorpis::vector<ThrowAfterN> test_vec;
        ThrowAfterN::reset();

        for (int val : original) {
            test_vec.push_back(ThrowAfterN(val));
        }

        size_t original_size = test_vec.size();
        ThrowAfterN::reset(throw_at);

        try {
            test_vec.insert(test_vec.begin() + 1,
                            {ThrowAfterN(10), ThrowAfterN(20), ThrowAfterN(30),
                             ThrowAfterN(40), ThrowAfterN(50)});
        } catch (const std::runtime_error&) {
            EXPECT_EQ(test_vec.size(), original_size);
            verifyContents(test_vec, original);
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertInitList_ExceptionSafety_VariousPositions) {
    std::vector<int> original = {1, 2, 3, 4, 5};
    std::vector<size_t> positions = {0, 2, 5};

    for (size_t pos : positions) {
        for (int throw_at = 1; throw_at < 8; ++throw_at) {
            ptorpis::vector<ThrowAfterN> test_vec;
            test_vec.reserve(20);
            ThrowAfterN::reset();

            for (int val : original) {
                test_vec.push_back(ThrowAfterN(val));
            }

            size_t original_size = test_vec.size();
            ThrowAfterN::reset(throw_at);

            try {
                test_vec.insert(test_vec.begin() + pos,
                                {ThrowAfterN(10), ThrowAfterN(20), ThrowAfterN(30)});
            } catch (const std::runtime_error&) {
                EXPECT_EQ(test_vec.size(), original_size);
                verifyContents(test_vec, original);
            }
        }
    }
}

TEST_F(VectorExceptionSafetyTest, InsertRange_ReturnIterator) {
    ptorpis::vector<int> vec = {1, 2, 3, 4};
    std::vector<int> to_insert = {10, 20};

    auto it = vec.insert(vec.begin() + 2, to_insert.begin(), to_insert.end());

    EXPECT_EQ(*it, 10);
    EXPECT_EQ(it, vec.begin() + 2);
}

TEST_F(VectorExceptionSafetyTest, InsertInitList_ReturnIterator) {
    ptorpis::vector<int> vec = {1, 2, 3, 4};

    auto it = vec.insert(vec.begin() + 2, {10, 20});

    EXPECT_EQ(*it, 10);
    EXPECT_EQ(it, vec.begin() + 2);
}
