#include "vector.hpp"
#include <gtest/gtest.h>
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
