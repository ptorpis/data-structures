/**
 * @file data-structures/spsc_queue/include/spsc_queue.hpp
 * @brief Implementation of a Single Producer - Single Consumer Lock-free & Thread safe
 * queue, using atomics
 * @author ptorpis -- Peter Torpis
 * Second data structure in this project after the implementation of vector
 * There is no standard container to compare this one 1 to 1, only 3rd party libraries
 */

#pragma once

#include <atomic>
#include <bit>
#include <cinttypes>
#include <cstddef>
#include <memory>

namespace ptorpis {
template <typename T, typename Allocator = std::allocator<T>> class spsc_queue {
    using alloc_traits = std::allocator_traits<Allocator>;
    using size_type = std::size_t;

public:
    explicit spsc_queue(size_type requested_capacity,
                        const Allocator& allocator = Allocator())
        : buffer_size_m(std::bit_ceil(requested_capacity)), mask_m(buffer_size_m - 1),
          head_m(0), tail_m(0), alloc_m(allocator) {
        buffer_m = alloc_traits::allocate(alloc_m, buffer_size_m);
    }

    ~spsc_queue() {
        size_type current_head = head_m.load(std::memory_order_relaxed);
        size_type current_tail = head_m.load(std::memory_order_relaxed);

        while (current_head != current_tail) {
            size_type index = current_head & mask_m;
            buffer_m[index].~T();
            ++current_head;
        }

        alloc_traits::deallocate(alloc_m, buffer_m, buffer_size_m);
    }

    bool try_push(const T& item) {
        size_type current_tail = tail_m.load(std::memory_order_relaxed);
        size_type current_head = head_m.load(std::memory_order_acquire);

        size_type next_tail = current_tail + 1;

        if (next_tail - current_head >= buffer_size_m) {
            return false; // full
        }

        size_type index = current_tail & mask_m;
        new (&buffer_m[index]) T(item);
        tail_m.store(next_tail, std::memory_order_release);
        return true;
    }

    bool try_push(T&& item) {
        size_type current_tail = tail_m.load(std::memory_order_relaxed);
        size_type current_head = head_m.load(std::memory_order_acquire);

        size_type next_tail = current_tail + 1;

        if (next_tail - current_head >= buffer_size_m) {
            return false; // full
        }

        size_type index = current_tail & mask_m;
        new (&buffer_m[index]) T(std::move(item));
        tail_m.store(next_tail, std::memory_order_release);
        return true;
    }

    bool try_pop(T& item) {
        size_type current_head = head_m.load(std::memory_order_relaxed);
        size_type current_tail = tail_m.load(std::memory_order_acquire);

        if (current_head == current_tail) {
            return false;
        }

        size_type index = current_head & mask_m;
        item = std::move(buffer_m[index]);
        buffer_m[index].~T();
        head_m.store(current_head + 1, std::memory_order_release);
        return true;
    }

    template <typename... Args> bool try_emplace(Args&&... args) {
        size_type current_tail = tail_m.load(std::memory_order_relaxed);
        size_type current_head = head_m.load(std::memory_order_acquire);
        size_type next_tail = current_tail + 1;
        if (next_tail - current_head >= buffer_size_m) {
            return false;
        }
        size_type index = current_tail & mask_m;
        new (&buffer_m[index]) T(std::forward<Args>(args)...);
        tail_m.store(next_tail, std::memory_order_release);
        return true;
    }

    size_type capacity() const noexcept { return buffer_size_m - 1; }

    bool full() const noexcept {
        size_type approx_head = head_m.load(std::memory_order_relaxed);
        size_type approx_tail = tail_m.load(std::memory_order_relaxed);

        if ((approx_tail + 1) - approx_head >= buffer_size_m) {
            return true;
        } else {
            return false;
        }
    }

    bool empty() const noexcept {

        size_type current_head = head_m.load(std::memory_order_relaxed);
        size_type current_tail = tail_m.load(std::memory_order_acquire);

        if (current_head == current_tail) {
            return true;
        } else {
            return false;
        }
    }

private:
    T* buffer_m;
    const size_type buffer_size_m;
    size_type mask_m;

    alignas(64) std::atomic<size_type> head_m; // consumer position
    alignas(64) std::atomic<size_type> tail_m; // producer position

    [[no_unique_address]] Allocator alloc_m;
};
} // namespace ptorpis
