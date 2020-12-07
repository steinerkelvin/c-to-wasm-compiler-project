#!/bin/bash
set -e

render_ast_exe="./bin/render-ast"
tests_base="./tests"

mkdir -p "${tests_base}/input"
mkdir -p "${tests_base}/ast-dot"

for test_file in "${tests_base}"/input/*.c; do

    basename=$(basename "${test_file}" ".c")
    echo -n "Rendering '${basename}'... "

    ast_sexp_file="${tests_base}/ast-dot/${basename}.txt"
    ast_dot_file="${tests_base}/ast-dot/${basename}.dot"
    ast_png_file="${tests_base}/ast-dot/${basename}.png"

    # Renderiza AST
    "${render_ast_exe}" <"${test_file}" >"${ast_sexp_file}" 2>/dev/null &&
        ./scripts/tree2dot <"${ast_sexp_file}" >"${ast_dot_file}" &&
        dot <"${ast_dot_file}" -T png >"${ast_png_file}" ||
        echo -n "failed rendering"

    echo
    echo
done
