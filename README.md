# c-to-wasm-compiler-project

Hopefully, a C to WebAssemply compiler.

[demo on browser](http://c2wasm-life.surge.sh/)

[![demo](./docs/demo.gif)](https://asciinema.org/a/dGx6qmir81vWzT8m8trrHjkFr)

## Build requirements

- Make
- Flex
- Bison
- Clang

## Runtime requirements

- [WABT: The WebAssembly Binary Toolkit](https://github.com/WebAssembly/wabt)
- Node.js

### How to install requirements

#### Arch Linux

    sudo pacman -S make flex bison clang
    sudo pacman -S nodejs wabt

#### Ubuntu

    sudo apt install -y make flex bison clang
    suto apt install -y nodejs

Get `wabt` from https://github.com/WebAssembly/wabt/releases or whatever. `wabt`
is also avaiable on Nixpkgs.

### Build

    make

### Test

    make test-parsing
    make test-codegen

Run a [console version][2048_c] of the 2048 game written in C compiled to Wasm:

    ./run_codegen_tests.sh 2048

[2048_c]: https://github.com/kelvinss/2048_c

### File structure

    src/ - "library" sources
    main/ - .cpp files that compile to executables
    runtime/ - JavaScript runtime
    tests-parsing/ - test files for parsing
        input/ - inputs
        ast-dot/ - rendered ASTs
        tokens/ - dumped tokens from input files
        tokens-expected/ - expected token dumps
        output/ - stdout and stderr outputs
        output-expected/ - expected outputs
    tests-codegen/ - test files for code generation

### Tested with versions

- Make: `4.3-3` (Arch); `4.1` (Ubuntu Bionic)
- Flex: `2.6.4-3` (Arch); `2.6.4-3` (Ubuntu Bionic)
- Bison: `3.7.2-1` (Arch); `3.7.2` (Nix)
- Clang: `11.0.0-1` (Arch); `6.0.0-1` (Ubuntu Bionic)
- Nodejs: `12.20.0-1` (Arch); `10.19.0` (Ubuntu Bionic)
- Wabt: `1.0.20-1 ` (Arch)

You can see the versions that are being used in your environment with the
following command:

    make show-versions

## Trivia

We have managed to _crash the Clang compiler_ in the course of programming this
project. Code is on the `clang-crash` branch. Maybe I'll trying to open a bug
report later. :P
