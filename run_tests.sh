#!/bin/bash
set -e

dump_tokens_exe="./bin/custom/dump-tokens"
render_ast_exe="./bin/render-ast"
compiler_exe="./bin/compiler"
tests_base="./tests"

mkdir -p "${tests_base}/input"
mkdir -p "${tests_base}/output"
mkdir -p "${tests_base}/output-expected"
mkdir -p "${tests_base}/tokens"
mkdir -p "${tests_base}/tokens-expected"
mkdir -p "${tests_base}/ast-dot"

for test_file in "${tests_base}"/input/*.c; do

    basename=$(basename "${test_file}" ".c")
    echo "Testing '${basename}'"

    tokens_file="${tests_base}/tokens/${basename}.tokens.txt"
    tokens_expected_file="${tests_base}/tokens-expected/${basename}.tokens.txt"

    ast_sexp_file="${tests_base}/ast-dot/${basename}.txt"
    ast_dot_file="${tests_base}/ast-dot/${basename}.dot"
    ast_png_file="${tests_base}/ast-dot/${basename}.png"

    output_file="${tests_base}/output/${basename}.out.txt"
    err_file="${tests_base}/output/${basename}.err.txt"
    output_expected_file="${tests_base}/output-expected/${basename}.out.txt"
    err_expected_file="${tests_base}/output-expected/${basename}.err.txt"

    # Faz dump dos tokens do arquivo de entrada
    "${dump_tokens_exe}" <"${test_file}" >"${tokens_file}" 2>&1 || true

    # Renderiza AST
    "${render_ast_exe}" <"${test_file}" >"${ast_sexp_file}" 2>/dev/null &&
        ./scripts/tree2dot <"${ast_sexp_file}" >"${ast_dot_file}" &&
        dot <"${ast_dot_file}" -T png >"${ast_png_file}" ||
        true

    # "${compiler_exe}" <"${test_file}" 2> >(tee "${err_file}" >&2) > >(tee "${output_file}") || true
    "${compiler_exe}" <"${test_file}" 2>"${err_file}" >"${output_file}" || true

    touch "${tokens_expected_file}"
    touch "${output_expected_file}"
    touch "${err_expected_file}"

    diff -w "${tokens_expected_file}" "${tokens_file}" || true
    diff -w "${output_expected_file}" "${output_file}" || true
    diff -w "${err_expected_file}" "${err_file}" || true

    echo
done
