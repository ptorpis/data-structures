#pragma once
#include <utility>

namespace ptorpis {
namespace detail {

template <typename T> struct Node {
    using node = Node<T>;

    template <class... Args> Node(node* next_node, Args&&... args)
        : next(next_node), value(std::forward<Args>(args)...) {}

    Node(node* next_node, const T& val) : next(next_node), value(val) {}

    Node(node* next_node, T&& val) : next(next_node), value(std::move(val)) {}

    node* next;
    T value;
};

} // namespace detail
} // namespace ptorpis