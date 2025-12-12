#pragma once

#include <compare>
#include <cstddef>
#include <iterator>

namespace ptorpis {
namespace detail {

template <typename T> class vector_iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    vector_iterator() : ptr_(nullptr) {}
    explicit vector_iterator(T* ptr) : ptr_(ptr) {}

    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }

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

    vector_iterator operator+(std::ptrdiff_t n) const {
        vector_iterator temp = *this;
        temp += n;
        return temp;
    }

    vector_iterator operator-(std::ptrdiff_t n) const {
        vector_iterator temp = *this;
        temp -= n;
        return temp;
    }

    std::ptrdiff_t operator-(const vector_iterator& other) const {
        return (ptr_ - other.ptr_);
    }

    reference operator[](std::ptrdiff_t n) const { return ptr_[n]; }

    bool operator==(const vector_iterator& other) const { return other.ptr_ == ptr_; }

    std::strong_ordering operator<=>(const vector_iterator& other) const {
        return ptr_ <=> other.ptr_;
    }

private:
    pointer ptr_;
};

template <typename T> class vector_const_iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    vector_const_iterator() : ptr_(nullptr) {}
    explicit vector_const_iterator(T* pointer) : ptr_(pointer) {}

    vector_const_iterator(const vector_iterator<T>& it) : ptr_(&(*it)) {}

    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }

    vector_const_iterator& operator++() {
        ++ptr_;
        return *this;
    }

    vector_const_iterator operator++(int) {
        vector_const_iterator temp = *this;
        ++ptr_;
        return temp;
    }

    vector_const_iterator operator--() {
        --ptr_;
        return *this;
    }

    vector_const_iterator operator--(int) {
        vector_const_iterator temp = *this;
        --ptr_;
        return temp;
    }

    vector_const_iterator& operator+=(difference_type n) {
        ptr_ += n;
        return *this;
    }

    vector_const_iterator& operator-=(difference_type n) {
        ptr_ -= n;
        return *this;
    }

    vector_const_iterator operator+(difference_type n) const {
        vector_const_iterator temp = *this;
        temp += n;
        return temp;
    }

    vector_const_iterator operator-(difference_type n) const {
        vector_const_iterator temp = *this;
        temp -= n;
        return temp;
    }

    std::strong_ordering operator<=>(const vector_const_iterator& other) const {
        return ptr_ <=> other.ptr_;
    }

    bool operator==(const vector_const_iterator& other) const {
        return other.ptr_ == ptr_;
    }

private:
    pointer ptr_;
};

template <typename T>
bool operator==(const vector_iterator<T>& lhs, const vector_const_iterator<T>& rhs) {
    return &(*lhs) == &(*rhs);
}
template <typename T>
bool operator==(const vector_const_iterator<T>& lhs, const vector_iterator<T>& rhs) {
    return &(*lhs) == &(*rhs);
}

template <typename T> std::strong_ordering
operator<=>(const vector_iterator<T>& lhs, const vector_const_iterator<T>& rhs) {
    return &(*lhs) <=> &(*rhs);
}
template <typename T> std::strong_ordering
operator<=>(const vector_const_iterator<T>& lhs, const vector_iterator<T>& rhs) {
    return &(*lhs) <=> &(*rhs);
}

template <typename T> std::ptrdiff_t operator-(const vector_const_iterator<T>& lhs,
                                               const vector_iterator<T>& rhs) {
    return &(*lhs) - &(*rhs);
}

template <typename T> std::ptrdiff_t operator-(const vector_iterator<T>& lhs,
                                               const vector_const_iterator<T>& rhs) {
    return &(*lhs) - &(*rhs);
}

} // namespace detail

} // namespace ptorpis
