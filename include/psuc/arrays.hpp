// Lab 4 — one-dimensional array algorithms.
//
// Every algorithm is written out by hand (no <algorithm> helpers) so the
// code still demonstrates the technique the lab asks for, while operating on
// std::vector / std::span instead of fixed-size C arrays that overflow.
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace psuc::arrays {

using Value = long long;

struct MinMax {
    Value min;
    Value max;
};

enum class Order : std::uint8_t { Ascending, Descending };

/// Smallest and largest element in a single pass; std::nullopt when empty.
[[nodiscard]] std::optional<MinMax> min_max(std::span<const Value> values);

/// Inserts `value` so that it ends up at position `index` (0 <= index <= size).
/// Throws std::out_of_range when the index is past the end.
void insert_at(std::vector<Value>& values, std::size_t index, Value value);

/// Removes the first element equal to `value`.
/// Returns the index it was removed from, or std::nullopt when not present.
std::optional<std::size_t> remove_first(std::vector<Value>& values, Value value);

/// Trial-division primality test (checks divisors up to sqrt(n)).
[[nodiscard]] bool is_prime(Value n);

/// All prime elements, in their original order (duplicates kept).
[[nodiscard]] std::vector<Value> primes_in(std::span<const Value> values);

/// In-place bubble sort with early exit when a pass makes no swaps.
void bubble_sort(std::vector<Value>& values, Order order);

/// In-place selection sort.
void selection_sort(std::vector<Value>& values, Order order);

/// True when every element is <= its successor.
[[nodiscard]] bool is_sorted_ascending(std::span<const Value> values);

/// Inserts `value` into an ascending sequence so it stays sorted.
/// Equal elements keep their relative order (the new value goes after them).
void insert_sorted(std::vector<Value>& values, Value value);

/// Iterative binary search over an ascending sequence.
/// Returns the index of a matching element, or std::nullopt.
[[nodiscard]] std::optional<std::size_t> binary_search(std::span<const Value> sorted, Value key);

/// Copy of `values` keeping only the first occurrence of each element.
[[nodiscard]] std::vector<Value> remove_duplicates(std::span<const Value> values);

}  // namespace psuc::arrays
