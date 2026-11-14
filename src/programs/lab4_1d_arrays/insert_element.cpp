// Lab 4.2a — Insert an element into an array at a given position.
#include <iostream>

#include "psuc/arrays.hpp"
#include "psuc/cli.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "insert_element",
    .summary = "insert an element into an array at a given (0-based) index",
    .input = "n (0..100000), n integers, the value to insert, then the index (0..n)",
    .example_in = "4\\n10 20 40 50\\n30 2\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Number of elements:", 0, psuc::cli::kMaxElements);
    auto values = s.read_ints("Enter the elements:", n);
    const auto value = s.read_int("Value to insert:");
    const auto index = s.read_count("Index to insert at (0-based):", 0, n);
    s.out() << "Original: " << psuc::cli::format_list(values) << '\n';
    psuc::arrays::insert_at(values, index, value);
    s.out() << "After inserting " << value << " at index " << index << ": "
            << psuc::cli::format_list(values) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
