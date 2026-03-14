#include "forward_list.hpp"
#include "lifetime.hpp"
#include <forward_list>
#include <print>

int main() {
    std::println("hello, world");

    ptorpis::forward_list<int> fwdList{};

    fwdList.emplace_front(1);
    fwdList.emplace_front(2);
    fwdList.emplace_front(3);

    for (const auto elem : fwdList) {
        std::println("{}", elem);
    }

    std::println("");

    fwdList.pop_front();

    for (const auto elem : fwdList) {
        std::println("{}", elem);
    }

    std::println("{}", fwdList.empty());

    std::println("ptorpis::forward_list impl:");
    ptorpis::forward_list<Lifetime> ltlist{};

    ltlist.emplace_front(1);
    ltlist.pop_front();

    std::println("std::forward_list impl:");

    std::forward_list<Lifetime> stdltList{};

    stdltList.emplace_front(1);
    stdltList.pop_front();
}