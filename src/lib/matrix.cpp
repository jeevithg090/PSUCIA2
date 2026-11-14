#include "psuc/matrix.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string>

namespace psuc::matrix {

namespace {

std::string shape(const Matrix& m) {
    return std::to_string(m.rows()) + "x" + std::to_string(m.cols());
}

void require_same_shape(const Matrix& a, const Matrix& b, const char* operation) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) {
        throw std::invalid_argument(std::string(operation) +
                                    " needs matrices of the same size (got " + shape(a) + " and " +
                                    shape(b) + ")");
    }
}

void require_square(const Matrix& m, const char* operation) {
    if (!m.is_square()) {
        throw std::invalid_argument(std::string(operation) + " needs a square matrix (got " +
                                    shape(m) + ")");
    }
}

}  // namespace

Matrix::Matrix(std::size_t rows, std::size_t cols, Value fill)
    : rows_(rows), cols_(cols), data_(rows * cols, fill) {}

Matrix::Matrix(std::initializer_list<std::initializer_list<Value>> rows)
    : rows_(rows.size()), cols_(rows.size() == 0 ? 0 : rows.begin()->size()) {
    data_.reserve(rows_ * cols_);
    for (const auto& row : rows) {
        if (row.size() != cols_) {
            throw std::invalid_argument("every row of a matrix must have the same length");
        }
        data_.insert(data_.end(), row.begin(), row.end());
    }
}

Value& Matrix::at(std::size_t row, std::size_t col) {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("matrix index out of range");
    }
    return data_[(row * cols_) + col];
}

Value Matrix::at(std::size_t row, std::size_t col) const {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range("matrix index out of range");
    }
    return data_[(row * cols_) + col];
}

Matrix add(const Matrix& a, const Matrix& b) {
    require_same_shape(a, b, "addition");
    Matrix result(a.rows(), a.cols());
    for (std::size_t i = 0; i < a.rows(); ++i) {
        for (std::size_t j = 0; j < a.cols(); ++j) {
            result.at(i, j) = a.at(i, j) + b.at(i, j);
        }
    }
    return result;
}

Matrix subtract(const Matrix& a, const Matrix& b) {
    require_same_shape(a, b, "subtraction");
    Matrix result(a.rows(), a.cols());
    for (std::size_t i = 0; i < a.rows(); ++i) {
        for (std::size_t j = 0; j < a.cols(); ++j) {
            result.at(i, j) = a.at(i, j) - b.at(i, j);
        }
    }
    return result;
}

Matrix multiply(const Matrix& a, const Matrix& b) {
    if (a.cols() != b.rows()) {
        throw std::invalid_argument("multiplication needs columns of A (" +
                                    std::to_string(a.cols()) + ") to equal rows of B (" +
                                    std::to_string(b.rows()) + ")");
    }
    Matrix result(a.rows(), b.cols());
    for (std::size_t i = 0; i < a.rows(); ++i) {
        for (std::size_t j = 0; j < b.cols(); ++j) {
            Value sum = 0;
            for (std::size_t k = 0; k < a.cols(); ++k) {
                sum += a.at(i, k) * b.at(k, j);
            }
            result.at(i, j) = sum;
        }
    }
    return result;
}

std::vector<Value> row_sums(const Matrix& m) {
    std::vector<Value> sums(m.rows(), 0);
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            sums[i] += m.at(i, j);
        }
    }
    return sums;
}

std::vector<Value> column_sums(const Matrix& m) {
    std::vector<Value> sums(m.cols(), 0);
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            sums[j] += m.at(i, j);
        }
    }
    return sums;
}

bool is_lower_triangular(const Matrix& m) {
    if (!m.is_square()) {
        return false;
    }
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = i + 1; j < m.cols(); ++j) {
            if (m.at(i, j) != 0) {
                return false;
            }
        }
    }
    return true;
}

bool is_upper_triangular(const Matrix& m) {
    if (!m.is_square()) {
        return false;
    }
    for (std::size_t i = 1; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < i; ++j) {
            if (m.at(i, j) != 0) {
                return false;
            }
        }
    }
    return true;
}

bool is_symmetric(const Matrix& m) {
    if (!m.is_square()) {
        return false;
    }
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = i + 1; j < m.cols(); ++j) {
            if (m.at(i, j) != m.at(j, i)) {
                return false;
            }
        }
    }
    return true;
}

Value trace(const Matrix& m) {
    require_square(m, "trace");
    Value sum = 0;
    for (std::size_t i = 0; i < m.rows(); ++i) {
        sum += m.at(i, i);
    }
    return sum;
}

double norm(const Matrix& m) {
    double sum_of_squares = 0.0;
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            const auto v = static_cast<double>(m.at(i, j));
            sum_of_squares += v * v;
        }
    }
    return std::sqrt(sum_of_squares);
}

void print(std::ostream& out, const Matrix& m) {
    std::size_t width = 1;
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            width = std::max(width, std::to_string(m.at(i, j)).size());
        }
    }
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            if (j > 0) {
                out << ' ';
            }
            out << std::setw(static_cast<int>(width)) << m.at(i, j);
        }
        out << '\n';
    }
}

}  // namespace psuc::matrix
