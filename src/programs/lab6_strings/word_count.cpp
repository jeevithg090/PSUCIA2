// Lab 6.1 — Count the number of words in a sentence (no string-handling functions).
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/strings.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "word_count",
    .summary = "count the words in a sentence (any run of spaces/tabs separates words)",
    .input = "one line of text",
    .example_in = "  the quick   brown fox \\n",
};

void solve(psuc::cli::Session& s) {
    const auto line = s.read_line("Enter a sentence:");
    s.out() << "Number of words: " << psuc::strings::count_words(line) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
