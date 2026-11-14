// Lab 5.4a — Check whether a square matrix is lower triangular.
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/matrix.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "lower_triangular",
    .summary = "check whether a square matrix is lower triangular (zeros above the diagonal)",
    .input = "order n of the square matrix (1..500), then n*n elements row by row",
    .example_in = "3\\n1 0 0\\n2 3 0\\n4 5 6\\n",
};

void solve(psuc::cli::Session& s) {
    const auto n = s.read_count("Order of the square matrix:", 1, psuc::cli::kMaxMatrixDim);
    const auto m = s.read_matrix("Elements of the matrix:", n, n);
    s.out() << "Matrix:\n";
    psuc::matrix::print(s.out(), m);
    s.out() << '\n'
            << (psuc::matrix::is_lower_triangular(m) ? "It is a lower triangular matrix."
                                                     : "It is not a lower triangular matrix.")
            << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
