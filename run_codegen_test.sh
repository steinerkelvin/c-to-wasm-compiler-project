#!/bin/bash

name="${1:-codegen}"

make && valgrind ./bin/compiler <"${name}.c" >"${name}.wat" &&
    ./runtime/run "${name}"
