// Lab 5.3 — Compute the row sums and column sums of a matrix.
#include <iostream>

#include "psuc/cli.hpp"
#include "psuc/matrix.hpp"

namespace {

constexpr psuc::cli::ProgramInfo kInfo{
    .name = "row_column_sum",
    .summary = "compute the sum of every row and every column of a matrix",
    .input = "rows and columns (1..500), then the elements row by row",
    .example_in = "2 3\\n1 2 3\\n4 5 6\\n",
};

void solve(psuc::cli::Session& s) {
    using psuc::cli::kMaxMatrixDim;
    namespace mx = psuc::matrix;
    const auto rows = s.read_count("Number of rows:", 1, kMaxMatrixDim);
    const auto cols = s.read_count("Number of columns:", 1, kMaxMatrixDim);
    const auto m = s.read_matrix("Elements of the matrix:", rows, cols);

    auto& out = s.out();
    out << "Matrix:\n";
    mx::print(out, m);
    out << '\n';
    const auto row_totals = mx::row_sums(m);
    for (std::size_t i = 0; i < row_totals.size(); ++i) {
        out << "Row " << i + 1 << " sum: " << row_totals[i] << '\n';
    }
    const auto col_totals = mx::column_sums(m);
    for (std::size_t j = 0; j < col_totals.size(); ++j) {
        out << "Column " << j + 1 << " sum: " << col_totals[j] << '\n';
    }
}

}  // namespace

int main(int argc, char** argv) {
    return psuc::cli::run(argc, argv, kInfo, solve);
}
