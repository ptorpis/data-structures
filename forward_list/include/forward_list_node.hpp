#pragma once
#include <utility>

namespace ptorpis {
namespace detail {

template <typename T> struct Node {
    using node = Node<T>;

    template <class... Args> Node(node* next_node, Args&&... args)
        : next(next_node), value(std::forward<Args>(args)...) {}

    node* next;
    T value;
};

} // namespace detail
} // namespace ptorpis