// Lab 4.4 — Arrange array elements in ascending/descending order using
// selection sort or bubble sort.
#include <iostream>

#include "psuc/arrays.hpp"
#include "psuc/cli.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "array_sort",
    .summary = "sort an array with bubble or selection sort, ascending or descending",
    .input =
        "n (0..100000), n integers, algorithm (1 = bubble, 2 = selection), "
        "order (1 = ascending, 2 = descending)",
    .example_in = "6\\n5 1 4 2 8 0\\n1 1\\n",
};

void solve(psuc::cli::Session& s) {
    using psuc::arrays::Order;
    const auto n = s.read_count("Number of elements:", 0, psuc::cli::kMaxElements);
    auto values = s.read_ints("Enter the elements:", n);
    const auto algorithm = s.read_int_in_range("Algorithm (1 = bubble, 2 = selection):", 1, 2);
    const auto order_choice = s.read_int_in_range("Order (1 = ascending, 2 = descending):", 1, 2);
    const Order order = order_choice == 1 ? Order::Ascending : Order::Descending;

    s.out() << "Original: " << psuc::cli::format_list(values) << '\n';
    if (algorithm == 1) {
        psuc::arrays::bubble_sort(values, order);
    } else {
        psuc::arrays::selection_sort(values, order);
    }
    s.out() << "Sorted (" << (algorithm == 1 ? "bubble" : "selection") << " sort, "
            << (order == Order::Ascending ? "ascending" : "descending")
            << "): " << psuc::cli::format_list(values) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
