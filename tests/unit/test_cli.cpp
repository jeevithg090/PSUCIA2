#include <array>
#include <sstream>
#include <string>
#include <string_view>

#include "psuc/cli.hpp"
#include "test_framework.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "demo",
    .summary = "demo program",
    .input = "n then n integers",
    .example_in = "2\\n1 2\\n",
};

struct Result {
    int code;
    std::string out;
    std::string err;
};

Result run(std::string_view input, std::span<const std::string_view> args = {}) {
    std::istringstream in{std::string(input)};
    std::ostringstream out;
    std::ostringstream err;
    const int code = psuc::cli::run_with_streams(
        args, kInfo,
        [](psuc::cli::Session& s) {
            const auto n = s.read_count("Count:", 1, 3);
            const auto values = s.read_ints("Values:", n);
            s.out() << psuc::cli::format_list(values) << '\n';
        },
        in, out, err);
    return {code, out.str(), err.str()};
}

}  // namespace

TEST_CASE("cli: happy path writes results to stdout and prompts to stderr") {
    const auto r = run("2\n5 6\n");
    CHECK_EQ(r.code, 0);
    CHECK_EQ(r.out, std::string("[5, 6]\n"));
    CHECK(r.err.find("Count:") != std::string::npos);
}

TEST_CASE("cli: --quiet suppresses prompts") {
    constexpr std::array<std::string_view, 1> args{"--quiet"};
    const auto r = run("1 9", args);
    CHECK_EQ(r.code, 0);
    CHECK_EQ(r.out, std::string("[9]\n"));
    CHECK(r.err.empty());
}

TEST_CASE("cli: --help prints usage and exits 0") {
    constexpr std::array<std::string_view, 1> args{"--help"};
    const auto r = run("", args);
    CHECK_EQ(r.code, 0);
    CHECK(r.out.find("Usage: demo") != std::string::npos);
}

TEST_CASE("cli: unknown option exits 2") {
    constexpr std::array<std::string_view, 1> args{"--bogus"};
    const auto r = run("", args);
    CHECK_EQ(r.code, 2);
    CHECK(r.err.find("unknown option '--bogus'") != std::string::npos);
}

TEST_CASE("cli: non-numeric, out-of-range and truncated input exit 1") {
    constexpr std::array<std::string_view, 1> quiet{"-q"};
    auto r = run("abc", quiet);
    CHECK_EQ(r.code, 1);
    CHECK(r.err.find("expected an integer but got 'abc'") != std::string::npos);

    r = run("9", quiet);
    CHECK_EQ(r.code, 1);
    CHECK(r.err.find("must be between 1 and 3") != std::string::npos);

    r = run("3 1 2", quiet);
    CHECK_EQ(r.code, 1);
    CHECK(r.err.find("unexpected end of input") != std::string::npos);
}

TEST_CASE("cli: format_list") {
    CHECK_EQ(psuc::cli::format_list(std::vector<long long>{}), std::string("[]"));
    CHECK_EQ(psuc::cli::format_list(std::vector<long long>{1, -2}), std::string("[1, -2]"));
}

TEST_CASE("cli: read_line handles leftover newline and CRLF") {
    std::istringstream in("3\r\nhello world\r\n");
    std::ostringstream out;
    psuc::cli::Session s(in, out, nullptr);
    CHECK_EQ(s.read_int(""), 3LL);
    CHECK_EQ(s.read_line(""), std::string("hello world"));
}
