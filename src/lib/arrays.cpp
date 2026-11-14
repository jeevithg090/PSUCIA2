#include "psuc/arrays.hpp"

#include <stdexcept>
#include <string>
#include <utility>

namespace psuc::arrays {

namespace {

bool in_order(Value a, Value b, Order order) {
    return order == Order::Ascending ? a <= b : a >= b;
}

}  // namespace

std::optional<MinMax> min_max(std::span<const Value> values) {
    if (values.empty()) {
        return std::nullopt;
    }
    MinMax result{values.front(), values.front()};
    for (const Value v : values.subspan(1)) {
        if (v < result.min) {
            result.min = v;
        }
        if (v > result.max) {
            result.max = v;
        }
    }
    return result;
}

void insert_at(std::vector<Value>& values, std::size_t index, Value value) {
    if (index > values.size()) {
        throw std::out_of_range("index " + std::to_string(index) + " is outside 0.." +
                                std::to_string(values.size()));
    }
    values.push_back(value);  // grow by one; the slot is overwritten below
    for (std::size_t i = values.size() - 1; i > index; --i) {
        values[i] = values[i - 1];
    }
    values[index] = value;
}

std::optional<std::size_t> remove_first(std::vector<Value>& values, Value value) {
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (values[i] == value) {
            for (std::size_t j = i; j + 1 < values.size(); ++j) {
                values[j] = values[j + 1];
            }
            values.pop_back();
            return i;
        }
    }
    return std::nullopt;
}

bool is_prime(Value n) {
    if (n < 2) {
        return false;
    }
    if (n < 4) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    // d <= n / d avoids the overflow that d * d <= n could hit for large n.
    for (Value d = 3; d <= n / d; d += 2) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}

std::vector<Value> primes_in(std::span<const Value> values) {
    std::vector<Value> primes;
    for (const Value v : values) {
        if (is_prime(v)) {
            primes.push_back(v);
        }
    }
    return primes;
}

void bubble_sort(std::vector<Value>& values, Order order) {
    const std::size_t n = values.size();
    for (std::size_t pass = 0; pass + 1 < n; ++pass) {
        bool swapped = false;
        for (std::size_t i = 0; i + 1 < n - pass; ++i) {
            if (!in_order(values[i], values[i + 1], order)) {
                std::swap(values[i], values[i + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            return;
        }
    }
}

void selection_sort(std::vector<Value>& values, Order order) {
    const std::size_t n = values.size();
    for (std::size_t i = 0; i + 1 < n; ++i) {
        std::size_t best = i;
        for (std::size_t j = i + 1; j < n; ++j) {
            if (!in_order(values[best], values[j], order)) {
                best = j;
            }
        }
        if (best != i) {
            std::swap(values[i], values[best]);
        }
    }
}

bool is_sorted_ascending(std::span<const Value> values) {
    for (std::size_t i = 1; i < values.size(); ++i) {
        if (values[i - 1] > values[i]) {
            return false;
        }
    }
    return true;
}

void insert_sorted(std::vector<Value>& values, Value value) {
    std::size_t index = 0;
    while (index < values.size() && values[index] <= value) {
        ++index;
    }
    insert_at(values, index, value);
}

std::optional<std::size_t> binary_search(std::span<const Value> sorted, Value key) {
    std::size_t low = 0;
    std::size_t high = sorted.size();  // half-open range [low, high)
    while (low < high) {
        const std::size_t mid = low + (high - low) / 2;
        if (sorted[mid] == key) {
            return mid;
        }
        if (sorted[mid] < key) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return std::nullopt;
}

std::vector<Value> remove_duplicates(std::span<const Value> values) {
    std::vector<Value> unique;
    for (const Value v : values) {
        bool seen = false;
        for (const Value u : unique) {
            if (u == v) {
                seen = true;
                break;
            }
        }
        if (!seen) {
            unique.push_back(v);
        }
    }
    return unique;
}

}  // namespace psuc::arrays
