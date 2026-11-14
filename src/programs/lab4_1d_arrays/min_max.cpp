// Lab 4.1 — Find the largest and smallest element in an array.
#include <iostream>

#include "psuc/arrays.hpp"
#include "psuc/cli.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "min_max",
    .summary = "find the largest and smallest element in an array",
    .input = "n (1..100000), then n integers",
    .example_in = "5\\n4 -2 9 0 7\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Number of elements:", 1, psuc::cli::kMaxElements);
    const auto values = s.read_ints("Enter the elements:", n);
    const auto result = psuc::arrays::min_max(values);
    if (!result) {  // unreachable: n >= 1, but keep the optional honest
        throw psuc::cli::InputError("the array is empty");
    }
    s.out() << "Array: " << psuc::cli::format_list(values) << '\n'
            << "Largest: " << result->max << '\n'
            << "Smallest: " << result->min << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
