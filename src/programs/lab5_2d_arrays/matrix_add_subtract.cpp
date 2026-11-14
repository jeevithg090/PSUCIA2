// Lab 5.1 — Read two matrices, add and subtract them, and display all matrices.
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/matrix.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "matrix_add_subtract",
    .summary = "add and subtract two matrices of the same size",
    .input = "rows and columns (1..500), then the elements of A and of B, row by row",
    .example_in = "2 2\\n1 2 3 4\\n5 6 7 8\\n",
};

void solve(psuc::cli::Session& s) {
    using psuc::cli::kMaxMatrixDim;
    namespace mx = psuc::matrix;
    const auto rows = s.read_count("Number of rows:", 1, kMaxMatrixDim);
    const auto cols = s.read_count("Number of columns:", 1, kMaxMatrixDim);
    const auto a = s.read_matrix("Elements of matrix A:", rows, cols);
    const auto b = s.read_matrix("Elements of matrix B:", rows, cols);

    auto& out = s.out();
    out << "Matrix A:\n";
    mx::print(out, a);
    out << "\nMatrix B:\n";
    mx::print(out, b);
    out << "\nA + B:\n";
    mx::print(out, mx::add(a, b));
    out << "\nA - B:\n";
    mx::print(out, mx::subtract(a, b));
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
