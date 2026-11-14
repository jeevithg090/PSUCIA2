#include <cmath>
#include <sstream>
#include <stdexcept>

#include "psuc/matrix.hpp"
#include "test_framework.hpp"

using psuc::matrix::Matrix;
using V = std::vector<long long>;

TEST_CASE("Matrix: construction, access, bounds") {
    Matrix m(2, 3, 1);
    CHECK_EQ(m.rows(), 2U);
    CHECK_EQ(m.cols(), 3U);
    CHECK(!m.is_square());
    m.at(1, 2) = 9;
    CHECK_EQ(m.at(1, 2), 9);
    CHECK_THROWS_AS(static_cast<void>(m.at(2, 0)), std::out_of_range);
    CHECK_THROWS_AS((Matrix{{1, 2}, {3}}), std::invalid_argument);
}

TEST_CASE("add and subtract") {
    const Matrix a{{1, 2}, {3, 4}};
    const Matrix b{{5, 6}, {7, 8}};
    CHECK(psuc::matrix::add(a, b) == (Matrix{{6, 8}, {10, 12}}));
    CHECK(psuc::matrix::subtract(a, b) == (Matrix{{-4, -4}, {-4, -4}}));
    CHECK_THROWS_AS(psuc::matrix::add(a, Matrix(2, 3)), std::invalid_argument);
}

TEST_CASE("multiply: true matrix product, not element-wise") {
    const Matrix a{{1, 2, 3}, {4, 5, 6}};
    const Matrix b{{7, 8}, {9, 10}, {11, 12}};
    CHECK(psuc::matrix::multiply(a, b) == (Matrix{{58, 64}, {139, 154}}));
    const Matrix sq{{1, 2}, {3, 4}};
    CHECK(psuc::matrix::multiply(sq, sq) == (Matrix{{7, 10}, {15, 22}}));
    CHECK_THROWS_AS(psuc::matrix::multiply(a, a), std::invalid_argument);
}

TEST_CASE("row_sums and column_sums on a non-square matrix") {
    const Matrix m{{1, 2, 3}, {4, 5, 6}};
    CHECK(psuc::matrix::row_sums(m) == (V{6, 15}));
    CHECK(psuc::matrix::column_sums(m) == (V{5, 7, 9}));
}

TEST_CASE("triangular checks") {
    const Matrix lower{{1, 0, 0}, {2, 3, 0}, {4, 5, 6}};
    const Matrix upper{{1, 2, 3}, {0, 4, 5}, {0, 0, 6}};
    const Matrix diag{{1, 0}, {0, 2}};
    CHECK(psuc::matrix::is_lower_triangular(lower));
    CHECK(!psuc::matrix::is_upper_triangular(lower));
    CHECK(psuc::matrix::is_upper_triangular(upper));
    CHECK(!psuc::matrix::is_lower_triangular(upper));
    CHECK(psuc::matrix::is_lower_triangular(diag));
    CHECK(psuc::matrix::is_upper_triangular(diag));
    CHECK(!psuc::matrix::is_lower_triangular(Matrix(2, 3)));
}

TEST_CASE("is_symmetric") {
    CHECK(psuc::matrix::is_symmetric(Matrix{{1, 7, 3}, {7, 4, 5}, {3, 5, 6}}));
    CHECK(!psuc::matrix::is_symmetric(Matrix{{1, 2}, {3, 1}}));
    CHECK(!psuc::matrix::is_symmetric(Matrix(1, 2)));
}

TEST_CASE("trace and norm") {
    const Matrix m{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    CHECK_EQ(psuc::matrix::trace(m), 15);
    CHECK(std::fabs(psuc::matrix::norm(m) - std::sqrt(285.0)) < 1e-12);
    CHECK(std::fabs(psuc::matrix::norm(Matrix{{3, 4}}) - 5.0) < 1e-12);
    CHECK_THROWS_AS(psuc::matrix::trace(Matrix(2, 3)), std::invalid_argument);
}

TEST_CASE("print aligns columns") {
    std::ostringstream os;
    psuc::matrix::print(os, Matrix{{1, -10}, {100, 2}});
    CHECK_EQ(os.str(), std::string("  1 -10\n100   2\n"));
}
