// Lab 6 — string algorithms written WITHOUT library string-handling functions.
//
// The lab forbids strlen/strcmp/strstr/toupper and friends, so every routine
// here walks the characters by hand. std::string / std::string_view are used
// only as safe containers (no fixed-size buffers that silently truncate).
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

namespace psuc::strings {

/// Number of characters before the end of the view, counted by hand.
[[nodiscard]] std::size_t length(std::string_view text);

/// Words are maximal runs of non-whitespace; handles leading, trailing and
/// repeated spaces/tabs, and returns 0 for blank input.
[[nodiscard]] std::size_t count_words(std::string_view text);

/// Swaps the case of ASCII letters (aBcDe -> AbCdE); other characters unchanged.
[[nodiscard]] std::string toggle_case(std::string_view text);

/// Characters in reverse order.
[[nodiscard]] std::string reverse(std::string_view text);

/// Index of the first occurrence of `needle` in `haystack` (naive search).
/// An empty needle matches at 0.
[[nodiscard]] std::optional<std::size_t> find_substring(std::string_view haystack,
                                                        std::string_view needle);

/// True when the text reads the same forwards and backwards (case-sensitive,
/// every character counts — exactly as the lab defines it).
[[nodiscard]] bool is_palindrome(std::string_view text);

/// Converts an optionally signed decimal string ("-42", "+7", "  12 ") to an
/// integer. Throws std::invalid_argument for non-digits / empty input and
/// std::out_of_range when the value does not fit in a long long.
[[nodiscard]] long long parse_int(std::string_view text);

}  // namespace psuc::strings
