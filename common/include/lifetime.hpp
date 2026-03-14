#pragma once
#include <print>
#include <utility>

struct Lifetime {
    struct Counters {
        int constructor;
        int destructor;
        int copy_construct;
        int move_construct;
        int copy_assign;
        int move_assign;

        void reset() { *this = {}; }

        void print() const {
            std::println("constructor:    {}", constructor);
            std::println("destructor:     {}", destructor);
            std::println("copy construct: {}", copy_construct);
            std::println("move construct: {}", move_construct);
            std::println("copy assign:    {}", copy_assign);
            std::println("move assign:    {}", move_assign);
        }
    };

    inline static Counters counters;

    Lifetime(const int value = 42) : value_(value) {
        std::println("Lifetime::Lifetime() [constructor]");
        ++counters.constructor;
    }
    ~Lifetime() {
        std::println("~Lifetime() [destructor]");
        ++counters.destructor;
    }

    Lifetime(const Lifetime& other) : value_(other.value_) {
        std::println("Lifetime(const Lifetime&) [copy constructor]");
        ++counters.copy_construct;
    }
    Lifetime(Lifetime&& other) : value_(std::exchange(other.value_, 0)) {
        std::println("Lifetime(Lifetime&&) [move constructor]");
        ++counters.move_construct;
    }
    Lifetime& operator=(const Lifetime& other) {
        std::println("Lifetime::operator=(const Lifetime&) [copy assignment]");
        value_ = other.value_;
        ++counters.copy_assign;
        return *this;
    }
    Lifetime& operator=(Lifetime&& other) {
        std::println("Lifetime::operator=(Lifetime&&) [move assingment]");
        if (this != &other) {
            value_ = std::exchange(other.value_, 0);
        }
        ++counters.move_assign;
        return *this;
    }

    int value() const noexcept { return value_; }

    void get_overload() && { std::println("&&"); }
    void get_overload() & { std::println("& non-const"); }
    void get_overload() const& { std::println("const&"); }

private:
    int value_;
};