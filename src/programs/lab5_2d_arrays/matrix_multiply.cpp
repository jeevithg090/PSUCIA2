// Lab 5.2 — Read two matrices, multiply them, and display all matrices.
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/matrix.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "matrix_multiply",
    .summary = "multiply an m x n matrix A by an n x p matrix B",
    .input =
        "rows and columns of A, elements of A, rows and columns of B, elements of B "
        "(columns of A must equal rows of B; dimensions 1..500)",
    .example_in = "2 3\\n1 2 3 4 5 6\\n3 2\\n7 8 9 10 11 12\\n",
};

void solve(psuc::cli::Session& s) {
    using psuc::cli::kMaxMatrixDim;
    namespace mx = psuc::matrix;
    const auto a_rows = s.read_count("Rows of A:", 1, kMaxMatrixDim);
    const auto a_cols = s.read_count("Columns of A:", 1, kMaxMatrixDim);
    const auto a = s.read_matrix("Elements of matrix A:", a_rows, a_cols);
    const auto b_rows = s.read_count("Rows of B:", 1, kMaxMatrixDim);
    if (b_rows != a_cols) {
        throw psuc::cli::InputError("cannot multiply: A has " + std::to_string(a_cols) +
                                    " columns but B has " + std::to_string(b_rows) + " rows");
    }
    const auto b_cols = s.read_count("Columns of B:", 1, kMaxMatrixDim);
    const auto b = s.read_matrix("Elements of matrix B:", b_rows, b_cols);

    auto& out = s.out();
    out << "Matrix A:\n";
    mx::print(out, a);
    out << "\nMatrix B:\n";
    mx::print(out, b);
    out << "\nA x B:\n";
    mx::print(out, mx::multiply(a, b));
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
