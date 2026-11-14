// Lab 4.3 — Print all the prime numbers in a given array.
#include <iostream>

#include "psuc/arrays.hpp"
#include "psuc/cli.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "primes",
    .summary = "print every prime number found in an array",
    .input = "n (0..100000), then n integers",
    .example_in = "8\\n1 2 3 4 15 17 21 23\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Number of elements:", 0, psuc::cli::kMaxElements);
    const auto values = s.read_ints("Enter the elements:", n);
    const auto primes = psuc::arrays::primes_in(values);
    if (primes.empty()) {
        s.out() << "No prime numbers in the array.\n";
    } else {
        s.out() << "Prime numbers: " << psuc::cli::format_list(primes) << '\n';
    }
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
