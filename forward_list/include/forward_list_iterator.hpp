#pragma once
#include "forward_list_node.hpp"

namespace ptorpis {
namespace detail {
template <typename T> class forward_list_iterator {
    using node = ptorpis::detail::Node<T>;

public:
    using reference = T&;
    using pointer = T*;

    forward_list_iterator(node* node_ptr) : ptr(node_ptr) {}

    forward_list_iterator operator++(int) {
        node* temp;
        ptr = ptr->next;
        return *temp;
    }

    forward_list_iterator& operator++() {
        ptr = ptr->next;
        return *this;
    }

    friend bool operator==(const forward_list_iterator& lhs,
                           const forward_list_iterator& rhs) {
        return lhs.ptr == rhs.ptr;
    }

    void operator--() = delete;
    void operator--(int) = delete;

    friend void operator<=>(const forward_list_iterator&,
                            const forward_list_iterator&) = delete;

    reference operator*() { return ptr->value; }

    pointer operator->() { return &(ptr->value); }

private:
    node* ptr;
};

} // namespace detail

} // namespace ptorpis