#include "psuc/cli.hpp"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace psuc::cli {

namespace {

bool stdin_is_terminal() {
#ifdef _WIN32
    return _isatty(0) != 0;
#else
    return isatty(STDIN_FILENO) != 0;
#endif
}

void print_help(std::ostream& out, const ProgramInfo& info) {
    out << info.name << " - " << info.summary << "\n\n"
        << "Usage: " << info.name << " [--quiet] < input\n\n"
        << "Input (from stdin, separated by spaces or newlines):\n"
        << "  " << info.input << "\n\n"
        << "Options:\n"
        << "  -q, --quiet   never print input prompts\n"
        << "                (prompts are shown on stderr only when stdin is a terminal)\n"
        << "  -h, --help    show this help and exit\n\n"
        << "Example:\n"
        << "  printf '" << info.example_in << "' | ./" << info.name << '\n';
}

}  // namespace

Session::Session(std::istream& in, std::ostream& out, std::ostream* prompts)
    : in_(in), out_(out), prompts_(prompts) {}

void Session::prompt(std::string_view text) {
    if (prompts_ != nullptr && !text.empty()) {
        *prompts_ << text << ' ' << std::flush;
    }
}

long long Session::read_int(std::string_view prompt_text) {
    prompt(prompt_text);
    long long value = 0;
    after_token_ = true;
    if (in_ >> value) {
        return value;
    }
    if (in_.eof()) {
        throw InputError("unexpected end of input (expected an integer)");
    }
    in_.clear();
    std::string token;
    in_ >> token;
    throw InputError("expected an integer but got '" + token + "'");
}

long long Session::read_int_in_range(std::string_view prompt_text, long long min, long long max) {
    const long long value = read_int(prompt_text);
    if (value < min || value > max) {
        throw InputError("value " + std::to_string(value) + " must be between " +
                         std::to_string(min) + " and " + std::to_string(max));
    }
    return value;
}

std::size_t Session::read_count(std::string_view prompt_text, std::size_t min, std::size_t max) {
    return static_cast<std::size_t>(
        read_int_in_range(prompt_text, static_cast<long long>(min), static_cast<long long>(max)));
}

std::vector<long long> Session::read_ints(std::string_view prompt_text, std::size_t count) {
    prompt(prompt_text);
    std::vector<long long> values;
    values.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        values.push_back(read_int(""));
    }
    return values;
}

matrix::Matrix Session::read_matrix(std::string_view prompt_text, std::size_t rows,
                                    std::size_t cols) {
    prompt(prompt_text);
    matrix::Matrix m(rows, cols);
    for (std::size_t i = 0; i < rows; ++i) {
        for (std::size_t j = 0; j < cols; ++j) {
            m.at(i, j) = read_int("");
        }
    }
    return m;
}

std::string Session::read_line(std::string_view prompt_text) {
    prompt(prompt_text);
    // A previous numeric read leaves the rest of its line (usually just the
    // newline, maybe "\r\n" or trailing blanks) in the stream; skip it so the
    // text line is not read as empty.
    if (after_token_) {
        after_token_ = false;
        while (in_.peek() == ' ' || in_.peek() == '\t' || in_.peek() == '\r') {
            in_.ignore();
        }
        if (in_.peek() == '\n') {
            in_.ignore();
        }
    }
    std::string line;
    if (!std::getline(in_, line) && line.empty() && in_.eof()) {
        throw InputError("unexpected end of input (expected a line of text)");
    }
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    return line;
}

int run_with_streams(std::span<const std::string_view> args, const ProgramInfo& info,
                     const std::function<void(Session&)>& body, std::istream& in, std::ostream& out,
                     std::ostream& err, bool interactive) {
    bool quiet = !interactive;
    for (const std::string_view arg : args) {
        if (arg == "-h" || arg == "--help") {
            print_help(out, info);
            return 0;
        }
        if (arg == "-q" || arg == "--quiet") {
            quiet = true;
        } else {
            err << info.name << ": unknown option '" << arg << "'\n"
                << "Try '" << info.name << " --help' for usage.\n";
            return 2;
        }
    }

    Session session(in, out, quiet ? nullptr : &err);
    const auto report = [&](const std::exception& e) {
        out << std::flush;
        if (!quiet) {
            err << '\n';  // finish the pending prompt line
        }
        err << info.name << ": error: " << e.what() << '\n';
        return 1;
    };
    try {
        body(session);
    } catch (const InputError& e) {
        return report(e);
    } catch (const std::logic_error& e) {  // invalid_argument, out_of_range, ...
        return report(e);
    }
    out << std::flush;
    return 0;
}

int run(int argc, char** argv, const ProgramInfo& info, const std::function<void(Session&)>& body) {
    std::vector<std::string_view> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    try {
        return run_with_streams(args, info, body, std::cin, std::cout, std::cerr,
                                stdin_is_terminal());
    } catch (const std::exception& e) {
        std::cerr << info.name << ": internal error: " << e.what() << '\n';
        return 1;
    }
}

std::string format_list(std::span<const long long> values) {
    std::ostringstream os;
    os << '[';
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << values[i];
    }
    os << ']';
    return os.str();
}

}  // namespace psuc::cli
