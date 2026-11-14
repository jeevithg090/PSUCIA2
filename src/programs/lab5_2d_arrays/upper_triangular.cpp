// Lab 5.4b — Check whether a square matrix is upper triangular.
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/matrix.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "upper_triangular",
    .summary = "check whether a square matrix is upper triangular (zeros below the diagonal)",
    .input = "order n of the square matrix (1..500), then n*n elements row by row",
    .example_in = "3\\n1 2 3\\n0 4 5\\n0 0 6\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Order of the square matrix:", 1, psuc::cli::kMaxMatrixDim);
    const auto m = s.read_matrix("Elements of the matrix:", n, n);
    s.out() << "Matrix:\n";
    psuc::matrix::print(s.out(), m);
    s.out() << '\n'
            << (psuc::matrix::is_upper_triangular(m) ? "It is an upper triangular matrix."
                                                     : "It is not an upper triangular matrix.")
            << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
