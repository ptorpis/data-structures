#include <cstddef>
#include <memory>
#include <utility>

namespace ptorpis {

namespace detail {
struct BaseNode {
    BaseNode() noexcept : prev(this), next(this) {}
    BaseNode* prev;
    BaseNode* next;
};

template <typename T> struct Node : BaseNode {
    T value;

    template <typename... Args> explicit Node(Args&&... args)
        : BaseNode(), value(std::forward<Args>(args)...) {}
};

} // namespace detail

template <typename T, typename Allocator = std::allocator<T>> class list {
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    // using iterator = some_type;
    // using const_iterator = some_type;
    // using reverse_iterator = std::reverse_iterator<iterator>;
    // using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    list();
    explicit list(const Allocator& alloc);
    explicit list(size_type count, const_reference value = T(),
                  const Allocator& alloc = Allocator());
    explicit list(size_type count, const Allocator& alloc = Allocator());

    template <typename InputIt>
    list(InputIt first, InputIt last, const Allocator& alloc = Allocator());

private:
    using node_type = detail::Node<T>;
    using base_node = detail::BaseNode;
    using node_allocator_type =
        typename std::allocator_traits<Allocator>::template rebind_alloc<node_type>;
    using node_alloc_traits = std::allocator_traits<node_allocator_type>;

    base_node sentinel_;
    size_type size_;
    [[no_unique_address]] node_allocator_type node_alloc_;

    node_type* create_node(const T& value);
    node_type* create_node(T&& value);
    template <typename... Args> node_type* create_node(Args&&... args);

    void destroy_node(base_node* node) noexcept;
    void link_before(base_node* pos, base_node* node) noexcept;
    void unlink(base_node* node) noexcept;
    // base_node* get_node(const_iterator it) const noexcept;
};
} // namespace ptorpis
