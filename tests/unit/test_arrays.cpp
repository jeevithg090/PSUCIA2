#include <stdexcept>
#include <vector>

#include "psuc/arrays.hpp"
#include "test_framework.hpp"

using psuc::arrays::Order;
using V = std::vector<long long>;

TEST_CASE("min_max: finds both extremes") {
    const V values{4, -2, 9, 0, 7};
    const auto r = psuc::arrays::min_max(values);
    CHECK(r.has_value());
    CHECK_EQ(r->min, -2);
    CHECK_EQ(r->max, 9);
}

TEST_CASE("min_max: single element and empty input") {
    const V one{5};
    CHECK_EQ(psuc::arrays::min_max(one)->min, 5);
    CHECK_EQ(psuc::arrays::min_max(one)->max, 5);
    CHECK(!psuc::arrays::min_max(V{}).has_value());
}

TEST_CASE("insert_at: front, middle, end") {
    V v{10, 20, 40};
    psuc::arrays::insert_at(v, 2, 30);
    CHECK(v == (V{10, 20, 30, 40}));
    psuc::arrays::insert_at(v, 0, 5);
    CHECK(v == (V{5, 10, 20, 30, 40}));
    psuc::arrays::insert_at(v, v.size(), 50);
    CHECK(v == (V{5, 10, 20, 30, 40, 50}));
}

TEST_CASE("insert_at: into empty array and invalid index") {
    V v;
    psuc::arrays::insert_at(v, 0, 1);
    CHECK(v == (V{1}));
    CHECK_THROWS_AS(psuc::arrays::insert_at(v, 3, 9), std::out_of_range);
}

TEST_CASE("remove_first: removes only the first match") {
    V v{3, 8, 5, 8, 1};
    CHECK_EQ(psuc::arrays::remove_first(v, 8).value(), 1U);
    CHECK(v == (V{3, 5, 8, 1}));
    CHECK(!psuc::arrays::remove_first(v, 42).has_value());
    CHECK(v == (V{3, 5, 8, 1}));
}

TEST_CASE("is_prime: small numbers, negatives and squares of primes") {
    for (long long n : {-7LL, 0LL, 1LL, 4LL, 9LL, 15LL, 25LL, 49LL, 1'000'000LL}) {
        CHECK(!psuc::arrays::is_prime(n));
    }
    for (long long n : {2LL, 3LL, 5LL, 7LL, 11LL, 97LL, 7919LL, 1'000'000'007LL}) {
        CHECK(psuc::arrays::is_prime(n));
    }
}

TEST_CASE("primes_in: keeps order and duplicates") {
    CHECK(psuc::arrays::primes_in(V{1, 2, 3, 4, 15, 17, 2}) == (V{2, 3, 17, 2}));
    CHECK(psuc::arrays::primes_in(V{1, 4, 6}).empty());
}

TEST_CASE("bubble_sort and selection_sort: both orders") {
    const V input{5, 1, 4, 2, 8, 0, 4, -3};
    const V asc{-3, 0, 1, 2, 4, 4, 5, 8};
    const V desc{8, 5, 4, 4, 2, 1, 0, -3};
    V a = input;
    psuc::arrays::bubble_sort(a, Order::Ascending);
    CHECK(a == asc);
    a = input;
    psuc::arrays::bubble_sort(a, Order::Descending);
    CHECK(a == desc);
    a = input;
    psuc::arrays::selection_sort(a, Order::Ascending);
    CHECK(a == asc);
    a = input;
    psuc::arrays::selection_sort(a, Order::Descending);
    CHECK(a == desc);
}

TEST_CASE("sorting: empty and single-element arrays") {
    V empty;
    psuc::arrays::bubble_sort(empty, Order::Ascending);
    psuc::arrays::selection_sort(empty, Order::Descending);
    CHECK(empty.empty());
    V one{7};
    psuc::arrays::selection_sort(one, Order::Ascending);
    CHECK(one == (V{7}));
}

TEST_CASE("is_sorted_ascending") {
    CHECK(psuc::arrays::is_sorted_ascending(V{}));
    CHECK(psuc::arrays::is_sorted_ascending(V{1, 1, 2}));
    CHECK(!psuc::arrays::is_sorted_ascending(V{2, 1}));
}

TEST_CASE("insert_sorted: start, middle, end, duplicates, empty") {
    V v{1, 3, 5};
    psuc::arrays::insert_sorted(v, 4);
    CHECK(v == (V{1, 3, 4, 5}));
    psuc::arrays::insert_sorted(v, 0);
    CHECK(v == (V{0, 1, 3, 4, 5}));
    psuc::arrays::insert_sorted(v, 9);
    CHECK(v == (V{0, 1, 3, 4, 5, 9}));
    psuc::arrays::insert_sorted(v, 3);
    CHECK(v == (V{0, 1, 3, 3, 4, 5, 9}));
    V empty;
    psuc::arrays::insert_sorted(empty, 2);
    CHECK(empty == (V{2}));
}

TEST_CASE("binary_search: finds every element and rejects absent keys") {
    const V v{2, 4, 6, 8, 10, 12, 14};
    for (std::size_t i = 0; i < v.size(); ++i) {
        CHECK_EQ(psuc::arrays::binary_search(v, v[i]).value(), i);
    }
    for (long long missing : {1LL, 3LL, 15LL, -100LL}) {
        CHECK(!psuc::arrays::binary_search(v, missing).has_value());
    }
    CHECK(!psuc::arrays::binary_search(V{}, 1).has_value());
}

TEST_CASE("remove_duplicates: lab example") {
    CHECK(psuc::arrays::remove_duplicates(V{3, 4, 1, 3, 5, 5, 3}) == (V{3, 4, 1, 5}));
    CHECK(psuc::arrays::remove_duplicates(V{}).empty());
    CHECK(psuc::arrays::remove_duplicates(V{7, 7, 7}) == (V{7}));
}
