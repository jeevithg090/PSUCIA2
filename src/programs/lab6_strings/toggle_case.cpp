// Lab 6.2 — Toggle the case of every character in a string.
// Example: aBcDe -> AbCdE
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/strings.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "toggle_case",
    .summary = "swap upper and lower case of every letter (aBcDe -> AbCdE)",
    .input = "one line of text",
    .example_in = "aBcDe\\n",
};

void solve(psuc::cli::Session& s) {
    const auto line = s.read_line("Enter a string:");
    s.out() << "Toggled: " << psuc::strings::toggle_case(line) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
