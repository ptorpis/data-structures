#pragma once
#include <memory>
#include <utility>

#include "forward_list_iterator.hpp"
#include "forward_list_node.hpp"

namespace ptorpis {

template <typename T, typename Allocator = std::allocator<T>> class forward_list {
    using node = ptorpis::detail::Node<T>;

public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using iterator = ptorpis::detail::forward_list_iterator<T>;

    forward_list() : head_(nullptr) {}

    ~forward_list() {
        while (head_) pop_front();
    }

    bool empty() const { return head_ ? false : true; }

    void push_front(const T& value) { head_ = new node(head_, value); }

    void push_front(T&& value) { head_ = new node(head_, std::move(value)); }

    template <class... Args> reference emplace_front(Args&&... args) {
        head_ = new node(head_, std::forward<Args>(args)...);
        return head_->value;
    }

    void pop_front() {
        node* temp = head_;
        head_ = temp->next;
        delete temp;
    }

    iterator begin() const { return iterator{head_}; }

    iterator end() const { return iterator{nullptr}; }

    void operator[](int) const = delete;
    void operator[](int) = delete;

private:
    node* head_;
};
} // namespace ptorpis