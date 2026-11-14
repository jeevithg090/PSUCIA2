// Lab 4.6 — Search for a given number in an array using binary search.
#include <iostream>

#include "psuc/arrays.hpp"
#include "psuc/cli.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "binary_search",
    .summary = "find a value in an ascending array using binary search",
    .input = "n (0..100000), n integers in ascending order, then the value to find",
    .example_in = "7\\n2 4 6 8 10 12 14\\n10\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Number of elements:", 0, psuc::cli::kMaxElements);
    const auto values = s.read_ints("Enter the elements in ascending order:", n);
    if (!psuc::arrays::is_sorted_ascending(values)) {
        throw psuc::cli::InputError("binary search needs the array in ascending order, got " +
                                    psuc::cli::format_list(values));
    }
    const auto key = s.read_int("Value to search for:");
    if (const auto index = psuc::arrays::binary_search(values, key)) {
        s.out() << key << " found at index " << *index << " (position " << *index + 1 << ").\n";
    } else {
        s.out() << key << " is not present in the array.\n";
    }
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
