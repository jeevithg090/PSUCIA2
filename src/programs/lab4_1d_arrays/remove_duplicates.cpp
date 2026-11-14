// Lab 4.7 — Delete all the duplicate elements of an array.
// Example: [3, 4, 1, 3, 5, 5, 3] -> [3, 4, 1, 5]
#include <iostream>

#include "psuc/arrays.hpp"
#include "psuc/cli.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "remove_duplicates",
    .summary = "remove duplicate elements, keeping the first occurrence of each",
    .input = "n (0..100000), then n integers",
    .example_in = "7\\n3 4 1 3 5 5 3\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Number of elements:", 0, psuc::cli::kMaxElements);
    const auto values = s.read_ints("Enter the elements:", n);
    const auto unique = psuc::arrays::remove_duplicates(values);
    s.out() << "Original: " << psuc::cli::format_list(values) << '\n'
            << "Without duplicates: " << psuc::cli::format_list(unique) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
