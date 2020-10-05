#!/bin/bash
set -e

exe="./bin/compiler"
dump_tokens_exe="./bin/dump-tokens"
tests_base="./tests"

mkdir -p "${tests_base}/input"
mkdir -p "${tests_base}/output"
mkdir -p "${tests_base}/tokens"

for test_file in "${tests_base}"/input/*.c; do

    basename=$(basename "${test_file}" ".c")
    echo "Testing '${basename}'"

    output_file="${tests_base}/output/${basename}.txt"
    err_file="${tests_base}/output/${basename}.err.txt"
    < "${test_file}" "${exe}" 2>"${err_file}" > "${output_file}" 
    cat "${err_file}"

    tokens_file="${tests_base}/tokens/${basename}.tokens.txt"
    < "${test_file}" "${dump_tokens_exe}" > "${tokens_file}" 

done
