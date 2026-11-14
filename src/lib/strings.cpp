#include "psuc/strings.hpp"

#include <limits>
#include <stdexcept>

namespace psuc::strings {

namespace {

constexpr int kCaseOffset = 'a' - 'A';

bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

bool is_lower(char c) {
    return c >= 'a' && c <= 'z';
}

bool is_upper(char c) {
    return c >= 'A' && c <= 'Z';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

}  // namespace

std::size_t length(std::string_view text) {
    std::size_t n = 0;
    for ([[maybe_unused]] const char c : text) {
        ++n;
    }
    return n;
}

std::size_t count_words(std::string_view text) {
    std::size_t words = 0;
    bool in_word = false;
    for (const char c : text) {
        if (is_space(c)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            ++words;
        }
    }
    return words;
}

std::string toggle_case(std::string_view text) {
    std::string result(text);
    for (char& c : result) {
        if (is_lower(c)) {
            c = static_cast<char>(c - kCaseOffset);
        } else if (is_upper(c)) {
            c = static_cast<char>(c + kCaseOffset);
        }
    }
    return result;
}

std::string reverse(std::string_view text) {
    std::string result(text);
    const std::size_t n = length(result);
    for (std::size_t i = 0; i < n / 2; ++i) {
        const char tmp = result[i];
        result[i] = result[n - i - 1];
        result[n - i - 1] = tmp;
    }
    return result;
}

std::optional<std::size_t> find_substring(std::string_view haystack, std::string_view needle) {
    const std::size_t h = length(haystack);
    const std::size_t n = length(needle);
    if (n > h) {
        return std::nullopt;
    }
    for (std::size_t start = 0; start + n <= h; ++start) {
        std::size_t matched = 0;
        while (matched < n && haystack[start + matched] == needle[matched]) {
            ++matched;
        }
        if (matched == n) {
            return start;
        }
    }
    return std::nullopt;
}

bool is_palindrome(std::string_view text) {
    const std::size_t n = length(text);
    for (std::size_t i = 0; i < n / 2; ++i) {
        if (text[i] != text[n - i - 1]) {
            return false;
        }
    }
    return true;
}

long long parse_int(std::string_view text) {
    std::size_t i = 0;
    const std::size_t n = length(text);
    while (i < n && is_space(text[i])) {
        ++i;
    }
    std::size_t end = n;
    while (end > i && is_space(text[end - 1])) {
        --end;
    }

    bool negative = false;
    if (i < end && (text[i] == '+' || text[i] == '-')) {
        negative = text[i] == '-';
        ++i;
    }
    if (i == end) {
        throw std::invalid_argument("expected at least one digit");
    }

    // Accumulate as a negative number: its range is one larger than the
    // positive range, so LLONG_MIN parses without overflowing.
    constexpr long long kMin = std::numeric_limits<long long>::min();
    long long value = 0;
    for (; i < end; ++i) {
        const char c = text[i];
        if (!is_digit(c)) {
            throw std::invalid_argument(std::string("'") + c + "' is not a digit");
        }
        const int digit = c - '0';
        if (value < (kMin + digit) / 10) {
            throw std::out_of_range("number does not fit in a 64-bit integer");
        }
        value = (value * 10) - digit;
    }
    if (!negative) {
        if (value == kMin) {
            throw std::out_of_range("number does not fit in a 64-bit integer");
        }
        value = -value;
    }
    return value;
}

}  // namespace psuc::strings
