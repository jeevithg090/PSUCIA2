// Lab 5 — two-dimensional array (matrix) algorithms.
#pragma once

#include <cstddef>
#include <initializer_list>
#include <iosfwd>
#include <vector>

namespace psuc::matrix {

using Value = long long;

/// Dense row-major matrix with bounds-checked element access.
class Matrix {
public:
    Matrix() = default;
    /// Creates a rows x cols matrix filled with `fill`.
    Matrix(std::size_t rows, std::size_t cols, Value fill = 0);
    /// Creates a matrix from nested rows; throws std::invalid_argument if ragged.
    Matrix(std::initializer_list<std::initializer_list<Value>> rows);

    [[nodiscard]] std::size_t rows() const noexcept { return rows_; }
    [[nodiscard]] std::size_t cols() const noexcept { return cols_; }
    [[nodiscard]] bool is_square() const noexcept { return rows_ == cols_; }

    /// Bounds-checked access; throws std::out_of_range.
    [[nodiscard]] Value& at(std::size_t row, std::size_t col);
    [[nodiscard]] Value at(std::size_t row, std::size_t col) const;

    friend bool operator==(const Matrix&, const Matrix&) = default;

private:
    std::size_t rows_ = 0;
    std::size_t cols_ = 0;
    std::vector<Value> data_;
};

/// Element-wise sum; throws std::invalid_argument when shapes differ.
[[nodiscard]] Matrix add(const Matrix& a, const Matrix& b);

/// Element-wise difference a - b; throws std::invalid_argument when shapes differ.
[[nodiscard]] Matrix subtract(const Matrix& a, const Matrix& b);

/// Matrix product a x b; throws std::invalid_argument when a.cols() != b.rows().
[[nodiscard]] Matrix multiply(const Matrix& a, const Matrix& b);

[[nodiscard]] std::vector<Value> row_sums(const Matrix& m);
[[nodiscard]] std::vector<Value> column_sums(const Matrix& m);

/// Square matrix whose entries above the main diagonal are all zero.
[[nodiscard]] bool is_lower_triangular(const Matrix& m);

/// Square matrix whose entries below the main diagonal are all zero.
[[nodiscard]] bool is_upper_triangular(const Matrix& m);

/// Square matrix equal to its transpose.
[[nodiscard]] bool is_symmetric(const Matrix& m);

/// Sum of the main diagonal; throws std::invalid_argument if not square.
[[nodiscard]] Value trace(const Matrix& m);

/// Frobenius norm: sqrt of the sum of squares of every element.
[[nodiscard]] double norm(const Matrix& m);

/// Writes the matrix with right-aligned columns, one row per line.
void print(std::ostream& out, const Matrix& m);

}  // namespace psuc::matrix
