#include <limits>
#include <stdexcept>
#include <string>

#include "psuc/strings.hpp"
#include "test_framework.hpp"

namespace str = psuc::strings;

TEST_CASE("length") {
    CHECK_EQ(str::length(""), 0U);
    CHECK_EQ(str::length("hello"), 5U);
}

TEST_CASE("count_words: irregular whitespace") {
    CHECK_EQ(str::count_words("the quick brown fox"), 4U);
    CHECK_EQ(str::count_words("  leading and trailing  "), 3U);
    CHECK_EQ(str::count_words("multiple   spaces\tand\ttabs"), 4U);
    CHECK_EQ(str::count_words(""), 0U);
    CHECK_EQ(str::count_words("    "), 0U);
    CHECK_EQ(str::count_words("one"), 1U);
}

TEST_CASE("toggle_case: lab example and non-letters") {
    CHECK_EQ(str::toggle_case("aBcDe"), std::string("AbCdE"));
    CHECK_EQ(str::toggle_case("Hello, World! 123"), std::string("hELLO, wORLD! 123"));
    CHECK_EQ(str::toggle_case(""), std::string());
}

TEST_CASE("reverse") {
    CHECK_EQ(str::reverse("hello"), std::string("olleh"));
    CHECK_EQ(str::reverse("ab"), std::string("ba"));
    CHECK_EQ(str::reverse(""), std::string());
}

TEST_CASE("find_substring") {
    CHECK_EQ(str::find_substring("problem solving", "solving").value(), 8U);
    CHECK_EQ(str::find_substring("aaab", "aab").value(), 1U);
    CHECK_EQ(str::find_substring("abc", "abc").value(), 0U);
    CHECK_EQ(str::find_substring("abc", "").value(), 0U);
    CHECK(!str::find_substring("abc", "abcd").has_value());
    CHECK(!str::find_substring("abc", "x").has_value());
    CHECK(!str::find_substring("", "a").has_value());
}

TEST_CASE("is_palindrome") {
    CHECK(str::is_palindrome("racecar"));
    CHECK(str::is_palindrome("abba"));
    CHECK(str::is_palindrome("a"));
    CHECK(str::is_palindrome(""));
    CHECK(!str::is_palindrome("hello"));
    CHECK(!str::is_palindrome("Abba"));  // case-sensitive
}

TEST_CASE("parse_int: valid input") {
    CHECK_EQ(str::parse_int("0"), 0LL);
    CHECK_EQ(str::parse_int("12345"), 12345LL);
    CHECK_EQ(str::parse_int("-42"), -42LL);
    CHECK_EQ(str::parse_int("+7"), 7LL);
    CHECK_EQ(str::parse_int("  56  "), 56LL);
    CHECK_EQ(str::parse_int("007"), 7LL);
    CHECK_EQ(str::parse_int("9223372036854775807"), std::numeric_limits<long long>::max());
    CHECK_EQ(str::parse_int("-9223372036854775808"), std::numeric_limits<long long>::min());
}

TEST_CASE("parse_int: invalid input") {
    CHECK_THROWS_AS(str::parse_int(""), std::invalid_argument);
    CHECK_THROWS_AS(str::parse_int("   "), std::invalid_argument);
    CHECK_THROWS_AS(str::parse_int("-"), std::invalid_argument);
    CHECK_THROWS_AS(str::parse_int("12a"), std::invalid_argument);
    CHECK_THROWS_AS(str::parse_int("1 2"), std::invalid_argument);
    CHECK_THROWS_AS(str::parse_int("3.14"), std::invalid_argument);
    CHECK_THROWS_AS(str::parse_int("9223372036854775808"), std::out_of_range);
    CHECK_THROWS_AS(str::parse_int("-9223372036854775809"), std::out_of_range);
}
