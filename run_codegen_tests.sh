#!/bin/bash

set -e

compiler_exe="./bin/compiler"
tests_base="./tests-codegen/"

use_valgrind= #1

mkdir -p "${tests_base}/build"

function run_test() {
    local name="$1"

    local source_file="${tests_base}/${name}.c"
    local wat_file="${tests_base}/build/${name}.wat"
    local wasm_file="${tests_base}/build/${name}.wasm"

    if [ -n "${use_valgrind}" ]; then
        valgrind "${compiler_exe}" <"${source_file}" >"${wat_file}"
    else
        "${compiler_exe}" <"${source_file}" >"${wat_file}"
    fi

    wat2wasm "${wat_file}" -o "${wasm_file}"
    ./runtime/runtime "${wasm_file}"
    echo
}

if [ -n "${1}" ]; then
    run_test "${1}"
else

    for test_file in "${tests_base}"/*.c; do
        name=$(basename "${test_file}" ".c")
        echo "Testing '${name}'"
        run_test "${name}"
    done

fi
