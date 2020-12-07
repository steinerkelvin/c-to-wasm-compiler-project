#!/bin/bash

name=codegen; make && valgrind ./bin/compiler < ${name}.c > ${name}.wat  && ./runtime/run $name
