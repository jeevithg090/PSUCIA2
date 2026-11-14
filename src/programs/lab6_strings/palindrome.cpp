// Lab 6.4 — Check whether a given string is a palindrome.
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/strings.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "palindrome",
    .summary = "check whether a string reads the same backwards (case-sensitive)",
    .input = "one line of text",
    .example_in = "racecar\\n",
};

void solve(psuc::cli::Session& s) {
    const auto line = s.read_line("Enter a string:");
    s.out() << "\"" << line << "\" "
            << (psuc::strings::is_palindrome(line) ? "is a palindrome." : "is not a palindrome.")
            << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
