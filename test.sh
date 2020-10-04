#!/bin/bash
set -e

make 
< test.c ./exe   # | tee "./output.c"
