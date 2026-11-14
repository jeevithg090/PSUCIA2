// Bonus — Reverse a string in place by swapping characters from both ends.
// (The original submission for 6.2 contained this program by mistake; it is
// kept here as its own exercise.)
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/strings.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "reverse_string",
    .summary = "reverse a string by swapping characters from both ends",
    .input = "one line of text",
    .example_in = "hello world\\n",
};

void solve(psuc::cli::Session& s) {
    const auto line = s.read_line("Enter a string:");
    s.out() << "Reversed: " << psuc::strings::reverse(line) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
