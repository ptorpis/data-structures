#pragma once

#include <cstddef>

namespace ptorpis {
template <typename T> class vector_iterator {
public:
    vector_iterator() : ptr_(nullptr) {}
    explicit vector_iterator(T* ptr) : ptr_(ptr) {}

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

    vector_iterator& operator++() {
        ++ptr_;
        return *this;
    }

    vector_iterator operator++(int) {
        vector_iterator temp = *this;
        ++ptr_;
        return temp;
    }
    vector_iterator operator--() {
        --ptr_;
        return *this;
    }
    vector_iterator operator--(int) {
        vector_iterator temp = *this;
        --ptr_;
        return temp;
    }

    vector_iterator& operator+=(std::ptrdiff_t n);

    vector_iterator& operator-=(std::ptrdiff_t n);
    vector_iterator operator+(std::ptrdiff_t n) const;
    vector_iterator operator-(std::ptrdiff_t n) const;
    std::ptrdiff_t operator-(const vector_iterator& other) const;

    T& operator[](std::ptrdiff_t n) const;

    bool operator==(const vector_iterator& other) const;
    bool operator!=(const vector_iterator& other) const { return other.ptr_ != ptr_; }
    bool operator<(const vector_iterator& other) const;
    bool operator<=(const vector_iterator& other) const;
    bool operator>(const vector_iterator& other) const;
    bool operator>=(const vector_iterator& other) const;

private:
    T* ptr_;
};

template <typename T> class vector_const_iterator {
private:
    const T* ptr_;
};

} // namespace ptorpis
