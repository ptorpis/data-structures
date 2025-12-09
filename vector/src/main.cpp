#include "vector.hpp"
#include <ios>
#include <iostream>
#include <print>
#include <vector>

int main() {
    ptorpis::vector<int> vec(5);

    std::vector<int> stdvec(5);

    std::println("std::vector capacity: {}", stdvec.capacity());
    std::println("custom vector capacity: {}", vec.capacity());

    std::println("std::vector size: {}", stdvec.size());
    std::println("custom vector size: {}", vec.size());

    for (auto v : stdvec) {
        std::cout << v << std::endl;
    }

    for (std::size_t i{}; i < 5; ++i) {
        std::cout << vec[i] << std::endl;
    }

    std::cout << "Is empty (std): " << std::boolalpha << stdvec.empty() << std::dec
              << std::endl;

    std::cout << "Is empty (custom): " << std::boolalpha << vec.empty() << std::dec
              << std::endl;

    std::cout << sizeof(stdvec) << std::endl << sizeof(vec) << std::endl;

    ptorpis::vector<int> v{1, 2, 3, 4, 5};

    for (auto elem : v) {
        std::cout << elem << " ";
    }

    std::cout << std::endl;

    return 0;
}
