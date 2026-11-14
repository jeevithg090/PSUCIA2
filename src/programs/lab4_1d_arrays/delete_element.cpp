// Lab 4.2b — Delete an element from an array.
#include <iostream>

#include "psuc/arrays.hpp"
#include "psuc/cli.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "delete_element",
    .summary = "delete the first occurrence of a value from an array",
    .input = "n (0..100000), n integers, then the value to delete",
    .example_in = "5\\n3 8 5 8 1\\n8\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Number of elements:", 0, psuc::cli::kMaxElements);
    auto values = s.read_ints("Enter the elements:", n);
    const auto value = s.read_int("Value to delete:");
    s.out() << "Original: " << psuc::cli::format_list(values) << '\n';
    if (const auto index = psuc::arrays::remove_first(values, value)) {
        s.out() << "Deleted " << value << " from index " << *index << ": "
                << psuc::cli::format_list(values) << '\n';
    } else {
        s.out() << value << " was not found in the array.\n";
    }
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
