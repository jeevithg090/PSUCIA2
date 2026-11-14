// Lab 5.6 — Find the trace and norm of a square matrix.
//   trace = sum of the principal diagonal elements
//   norm  = sqrt(sum of squares of every element)   (Frobenius norm)
#include <iomanip>
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/matrix.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "trace_norm",
    .summary = "compute the trace and the (Frobenius) norm of a square matrix",
    .input = "order n of the square matrix (1..500), then n*n elements row by row",
    .example_in = "3\\n1 2 3\\n4 5 6\\n7 8 9\\n",
};

void solve(psuc::cli::Session& s) {
    namespace mx = psuc::matrix;
    const auto n = s.read_count("Order of the square matrix:", 1, psuc::cli::kMaxMatrixDim);
    const auto m = s.read_matrix("Elements of the matrix:", n, n);
    auto& out = s.out();
    out << "Matrix:\n";
    mx::print(out, m);
    out << "\nTrace: " << mx::trace(m) << '\n'
        << "Norm: " << std::fixed << std::setprecision(4) << mx::norm(m) << '\n';
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
