// Minimal dependency-free unit test framework (a few macros over a registry).
#pragma once

#include <cstddef>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace psuc::test {

struct Failure {
    std::string message;
};

struct Case {
    const char* name;
    std::function<void()> body;
};

inline std::vector<Case>& registry() {
    static std::vector<Case> cases;
    return cases;
}

struct Registrar {
    Registrar(const char* name, std::function<void()> body) {
        registry().push_back({name, std::move(body)});
    }
};

template <typename A, typename B>
void check_eq(const A& actual, const B& expected, const char* expr, const char* file, int line) {
    if (!(actual == expected)) {
        std::ostringstream os;
        os << file << ':' << line << ": CHECK_EQ(" << expr << ") failed";
        if constexpr (requires(std::ostream& o) { o << actual << expected; }) {
            os << "\n    actual:   " << actual << "\n    expected: " << expected;
        }
        throw Failure{os.str()};
    }
}

inline int run_all() {
    std::size_t failed = 0;
    for (const auto& c : registry()) {
        try {
            c.body();
            std::cout << "[ PASS ] " << c.name << '\n';
        } catch (const Failure& f) {
            ++failed;
            std::cout << "[ FAIL ] " << c.name << "\n  " << f.message << '\n';
        } catch (const std::exception& e) {
            ++failed;
            std::cout << "[ FAIL ] " << c.name << "\n  unexpected exception: " << e.what() << '\n';
        }
    }
    std::cout << '\n'
              << registry().size() - failed << '/' << registry().size() << " tests passed\n";
    return failed == 0 ? 0 : 1;
}

}  // namespace psuc::test

#define PSUC_CONCAT_INNER(a, b) a##b
#define PSUC_CONCAT(a, b) PSUC_CONCAT_INNER(a, b)

#define TEST_CASE(name)                                                  \
    static void PSUC_CONCAT(test_fn_, __LINE__)();                       \
    static const psuc::test::Registrar PSUC_CONCAT(test_reg_, __LINE__)( \
        name, PSUC_CONCAT(test_fn_, __LINE__));                          \
    static void PSUC_CONCAT(test_fn_, __LINE__)()

#define CHECK(cond)                                                                            \
    do {                                                                                       \
        if (!(cond)) {                                                                         \
            throw psuc::test::Failure{std::string(__FILE__) + ":" + std::to_string(__LINE__) + \
                                      ": CHECK(" #cond ") failed"};                            \
        }                                                                                      \
    } while (false)

#define CHECK_EQ(actual, expected) \
    psuc::test::check_eq((actual), (expected), #actual ", " #expected, __FILE__, __LINE__)

#define CHECK_THROWS_AS(expr, ExceptionType)                                                   \
    do {                                                                                       \
        bool psuc_thrown = false;                                                              \
        try {                                                                                  \
            static_cast<void>(expr);                                                           \
        } catch (const ExceptionType&) {                                                       \
            psuc_thrown = true;                                                                \
        }                                                                                      \
        if (!psuc_thrown) {                                                                    \
            throw psuc::test::Failure{std::string(__FILE__) + ":" + std::to_string(__LINE__) + \
                                      ": expected " #ExceptionType " from " #expr};            \
        }                                                                                      \
    } while (false)
