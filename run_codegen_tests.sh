#!/bin/bash

set -e

name="${1:-codegen}"

compiler_exe="./bin/compiler"
tests_base="./tests-parsing/"

make && valgrind "${compiler_exe}" <"${name}.c" >"${name}.wat"
./runtime/run "${name}"
