#!/bin/bash
set -e

dump_tokens_exe="./bin/custom/dump-tokens"
parser_exe="./bin/just-parse"

tests_base="./tests-parsing/"

mkdir -p "${tests_base}/input"
mkdir -p "${tests_base}/output"
mkdir -p "${tests_base}/output-expected"
mkdir -p "${tests_base}/tokens"
mkdir -p "${tests_base}/tokens-expected"

for test_file in "${tests_base}"/input/*.c; do

    basename=$(basename "${test_file}" ".c")
    echo "Testing '${basename}'"

    tokens_file="${tests_base}/tokens/${basename}.tokens.txt"
    tokens_expected_file="${tests_base}/tokens-expected/${basename}.tokens.txt"

    output_file="${tests_base}/output/${basename}.out.txt"
    err_file="${tests_base}/output/${basename}.err.txt"
    output_expected_file="${tests_base}/output-expected/${basename}.out.txt"
    err_expected_file="${tests_base}/output-expected/${basename}.err.txt"

    # Faz dump dos tokens do arquivo de entrada
    "${dump_tokens_exe}" <"${test_file}" >"${tokens_file}" 2>&1 || true

    # "${parser_exe}" <"${test_file}" 2> >(tee "${err_file}" >&2) > >(tee "${output_file}") || true
    "${parser_exe}" <"${test_file}" >"${output_file}" 2>"${err_file}" || true

    touch "${tokens_expected_file}"
    touch "${output_expected_file}"
    touch "${err_expected_file}"

    diff -w "${tokens_expected_file}" "${tokens_file}" || true
    diff -w "${output_expected_file}" "${output_file}" || true
    diff -w "${err_expected_file}" "${err_file}" || true

    echo
done
