// Lab 6.5 — Convert a string representing a number to an integer.
#include <iostream>
#include <limits>
#include <stdexcept>

#include "psuc/cli.hpp"
#include "psuc/strings.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "string_to_int",
    .summary = "convert a decimal string such as \"-123\" to an integer",
    .input = "one line containing an optionally signed whole number",
    .example_in = "-123\\n",
};

void solve(psuc::cli::Session& s) {
    const auto line = s.read_line("Enter a number:");
    try {
        const long long value = psuc::strings::parse_int(line);
        s.out() << "Integer value: " << value << '\n';
        // Doing arithmetic proves the result is a real number, not text.
        if (value < std::numeric_limits<long long>::max()) {
            s.out() << "Value + 1: " << value + 1 << '\n';
        }
    } catch (const std::invalid_argument& e) {
        throw psuc::cli::InputError("\"" + line + "\" is not a valid integer: " + e.what());
    } catch (const std::out_of_range& e) {
        throw psuc::cli::InputError("\"" + line + "\" is out of range: " + e.what());
    }
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
