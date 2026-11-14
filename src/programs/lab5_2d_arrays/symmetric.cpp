// Lab 5.5 — Check whether a square matrix is symmetric (A equals its transpose).
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/matrix.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "symmetric",
    .summary = "check whether a square matrix is symmetric (A = transpose of A)",
    .input = "order n of the square matrix (1..500), then n*n elements row by row",
    .example_in = "3\\n1 7 3\\n7 4 5\\n3 5 6\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Order of the square matrix:", 1, psuc::cli::kMaxMatrixDim);
    const auto m = s.read_matrix("Elements of the matrix:", n, n);
    s.out() << "Matrix:\n";
    psuc::matrix::print(s.out(), m);
    s.out() << '\n'
            << (psuc::matrix::is_symmetric(m) ? "It is a symmetric matrix."
                                              : "It is not a symmetric matrix.")
            << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
