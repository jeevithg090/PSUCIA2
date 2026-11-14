// Shared command-line plumbing for every lab program.
//
// Conventions (identical for all programs):
//   * Input is read from stdin, whitespace separated, in the order listed by --help.
//   * Results go to stdout. Prompts go to stderr, and only when stdin is a
//     terminal — so `prog < input.txt > out.txt` yields clean output while an
//     interactive user is still guided. Pass --quiet (-q) to hide them always.
//   * Exit codes: 0 success, 1 invalid input, 2 bad command-line usage.
#pragma once

#include <cstddef>
#include <functional>
#include <iosfwd>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "psuc/matrix.hpp"

namespace psuc::cli {

/// Raised for malformed or out-of-range user input; reported as "error: ...".
class InputError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

struct ProgramInfo {
    std::string_view name;        // executable name, e.g. "min_max"
    std::string_view summary;     // one-line description
    std::string_view input;       // description of the expected stdin
    std::string_view example_in;  // sample stdin (for --help)
};

/// Upper bounds that keep accidental huge inputs from exhausting memory.
inline constexpr std::size_t kMaxElements = 100'000;
inline constexpr std::size_t kMaxMatrixDim = 500;

/// Typed, validated reads from an input stream with optional prompts.
class Session {
public:
    Session(std::istream& in, std::ostream& out, std::ostream* prompts);

    [[nodiscard]] std::ostream& out() { return out_; }

    /// Reads one integer. Throws InputError on bad input or end of input.
    long long read_int(std::string_view prompt);

    /// Reads an integer within [min, max].
    long long read_int_in_range(std::string_view prompt, long long min, long long max);

    /// Reads a count in [min, max] (used for array sizes and matrix dimensions).
    std::size_t read_count(std::string_view prompt, std::size_t min, std::size_t max);

    /// Reads `count` integers.
    std::vector<long long> read_ints(std::string_view prompt, std::size_t count);

    /// Reads a rows x cols matrix, row by row.
    matrix::Matrix read_matrix(std::string_view prompt, std::size_t rows, std::size_t cols);

    /// Reads a whole line (may be empty). A trailing '\r' is dropped.
    std::string read_line(std::string_view prompt);

private:
    void prompt(std::string_view text);

    std::istream& in_;
    std::ostream& out_;
    std::ostream* prompts_;
    bool after_token_ = false;
};

/// Parses arguments, handles --help/--quiet, runs `body` and maps errors to
/// exit codes. Use as: `return psuc::cli::run(argc, argv, kInfo, body);`
int run(int argc, char** argv, const ProgramInfo& info, const std::function<void(Session&)>& body);

/// Same as run() but with explicit streams; used by the tests.
/// `interactive` mirrors "stdin is a terminal" and enables prompts.
int run_with_streams(std::span<const std::string_view> args, const ProgramInfo& info,
                     const std::function<void(Session&)>& body, std::istream& in, std::ostream& out,
                     std::ostream& err, bool interactive = true);

/// Formats values as "[1, 2, 3]".
[[nodiscard]] std::string format_list(std::span<const long long> values);

}  // namespace psuc::cli
