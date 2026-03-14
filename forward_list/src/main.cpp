#include "forward_list.hpp"
#include <print>

int main() {
    std::println("hello, world");

    ptorpis::forward_list<int> fwdList{};

    fwdList.emplace_front(1);
    fwdList.emplace_front(2);

    for (const auto elem : fwdList) {
        std::println("{}", elem);
    }

    std::println("{}", fwdList.empty());
}