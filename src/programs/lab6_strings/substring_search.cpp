// Lab 6.3 — Search for a given substring in the main string.
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/strings.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "substring_search",
    .summary = "find the first position of a substring inside a string",
    .input = "the main string on one line, then the substring on the next line",
    .example_in = "problem solving using c\\nsolving\\n",
};

void solve(psuc::cli::Session& s) {
    const auto text = s.read_line("Enter the main string:");
    const auto pattern = s.read_line("Enter the substring to find:");
    if (const auto index = psuc::strings::find_substring(text, pattern)) {
        s.out() << "\"" << pattern << "\" found at index " << *index << " (position " << *index + 1
                << ").\n";
    } else {
        s.out() << "\"" << pattern << "\" was not found.\n";
    }
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
