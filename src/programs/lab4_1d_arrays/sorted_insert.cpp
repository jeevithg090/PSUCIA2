// Lab 4.5 — Insert an element into a sorted array so it remains sorted.
#include <iostream>

#include "psuc/arrays.hpp"
#include "psuc/cli.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "sorted_insert",
    .summary = "insert a value into an ascending array, keeping it sorted",
    .input = "n (0..100000), n integers in ascending order, then the value to insert",
    .example_in = "5\\n1 3 5 7 9\\n6\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Number of elements:", 0, psuc::cli::kMaxElements);
    auto values = s.read_ints("Enter the elements in ascending order:", n);
    if (!psuc::arrays::is_sorted_ascending(values)) {
        throw psuc::cli::InputError("the array must be in ascending order, got " +
                                    psuc::cli::format_list(values));
    }
    const auto value = s.read_int("Value to insert:");
    psuc::arrays::insert_sorted(values, value);
    s.out() << "After inserting " << value << ": " << psuc::cli::format_list(values) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
