#pragma once

#include <cstddef>

namespace ptorpis {
namespace detail {

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

    vector_iterator& operator+=(std::ptrdiff_t n) {
        ptr_ += n;
        return *this;
    }

    vector_iterator& operator-=(std::ptrdiff_t n) {
        ptr_ -= n;
        return *this;
    }

    vector_iterator operator+(std::ptrdiff_t n) const;
    vector_iterator operator-(std::ptrdiff_t n) const;
    std::ptrdiff_t operator-(const vector_iterator& other) const;

    T& operator[](std::ptrdiff_t n) const { return ptr_[n]; }

    bool operator==(const vector_iterator& other) const { return other.ptr_ == ptr_; }
    bool operator!=(const vector_iterator& other) const { return other.ptr_ != ptr_; }
    bool operator<(const vector_iterator& other) const;
    bool operator<=(const vector_iterator& other) const;
    bool operator>(const vector_iterator& other) const;
    bool operator>=(const vector_iterator& other) const;

private:
    T* ptr_;
};

template <typename T> class vector_const_iterator {
public:
    vector_const_iterator() : ptr_(nullptr) {}
    explicit vector_const_iterator(T* pointer) : ptr_(pointer) {}

    const T& operator*() const { return *ptr_; }
    const T& operator->() const { return ptr_; }

    const vector_const_iterator& operator++() {
        ++ptr_;
        return this;
    }

    const vector_const_iterator operator++(int) {
        vector_const_iterator temp = *this;
        ++ptr_;
        return temp;
    }

    const vector_const_iterator operator--() {
        --ptr_;
        return *this;
    }

    const vector_const_iterator operator--(int) {
        vector_const_iterator temp = *this;
        --ptr_;
        return temp;
    }

    const vector_const_iterator& operator+=(std::ptrdiff_t n) {
        ptr_ += n;
        return *this;
    }

    const vector_const_iterator& operator-=(std::ptrdiff_t n) {
        ptr_ -= n;
        return *this;
    }

private:
    const T* ptr_;
};

template <typename T> class reverse_iterator {
public:
    reverse_iterator() : ptr_(nullptr) {}
    explicit reverse_iterator(T* pointer) : ptr_(pointer) {}

private:
    T* ptr_;
};

} // namespace detail

} // namespace ptorpis
