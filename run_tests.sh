#!/bin/bash
set -e

exe="./bin/compiler"
dump_tokens_exe="./bin/custom/dump-tokens"
tests_base="./tests"

mkdir -p "${tests_base}/input"
mkdir -p "${tests_base}/output"
mkdir -p "${tests_base}/tokens"
mkdir -p "${tests_base}/output-expected"
mkdir -p "${tests_base}/tokens-expected"

for test_file in "${tests_base}"/input/*.c; do

    basename=$(basename "${test_file}" ".c")
    echo "Testing '${basename}'"

    tokens_file="${tests_base}/tokens/${basename}.tokens.txt"
    output_file="${tests_base}/output/${basename}.out.txt"
    err_file="${tests_base}/output/${basename}.err.txt"

    tokens_expected_file="${tests_base}/tokens-expected/${basename}.tokens.txt"
    output_expected_file="${tests_base}/output-expected/${basename}.out.txt"
    err_expected_file="${tests_base}/output-expected/${basename}.err.txt"

    < "${test_file}" "${dump_tokens_exe}" >"${tokens_file}" 2>&1 || true

    # < "${test_file}" "${exe}" 2> >(tee "${err_file}" >&2) > >(tee "${output_file}") || true
    < "${test_file}" "${exe}" 2> "${err_file}" > "${output_file}" || true

    touch "${tokens_expected_file}"
    touch "${output_expected_file}"
    touch "${err_expected_file}"

    diff -w "${tokens_expected_file}" "${tokens_file}"  || true
    diff -w "${output_expected_file}" "${output_file}"  || true
    diff -w "${err_expected_file}"    "${err_file}"     || true

    echo
done
