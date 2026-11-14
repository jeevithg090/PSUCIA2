#!/usr/bin/env bash
# Full local quality gate: configure, build (warnings are errors), check
# formatting, run clang-tidy and run every test.
#
# Usage: scripts/check.sh [build-dir]     (default: build/check)
set -euo pipefail

cd "$(dirname "$0")/.."
build_dir="${1:-build/check}"

generator=()
if command -v ninja >/dev/null 2>&1; then
    generator=(-G Ninja)
fi

echo "==> Configuring (${build_dir})"
cmake -S . -B "${build_dir}" "${generator[@]}" -DCMAKE_BUILD_TYPE=Debug >/dev/null

echo "==> Building"
cmake --build "${build_dir}"

if command -v clang-format >/dev/null 2>&1; then
    echo "==> Checking formatting"
    cmake --build "${build_dir}" --target format-check
else
    echo "==> clang-format not found, skipping format check"
fi

if command -v clang-tidy >/dev/null 2>&1; then
    echo "==> Running clang-tidy"
    cmake --build "${build_dir}" --target tidy
else
    echo "==> clang-tidy not found, skipping static analysis"
fi

echo "==> Running tests"
ctest --test-dir "${build_dir}" --output-on-failure

echo "==> All checks passed"
