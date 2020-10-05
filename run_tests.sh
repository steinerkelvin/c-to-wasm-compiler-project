#!/bin/bash
set -e

exe="./bin/compiler"
tests_base="./tests"

mkdir -p "${tests_base}/input"
mkdir -p "${tests_base}/output"

for test_file in "${tests_base}"/input/*.c; do

    basename=$(basename "${test_file}" ".c")
    echo "Testing '${basename}'"

    output_file="${tests_base}/output/${basename}.txt"
    err_file="${tests_base}/output/${basename}.err.txt"
    < "${test_file}" "${exe}" 2>"${err_file}" | tee "${output_file}" 
    cat "${err_file}"

done
