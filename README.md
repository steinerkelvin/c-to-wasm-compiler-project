# compiladores-trabalho

Hopefully, a C to WebAssemply compiler.

## Guidelines

- Test files with well-formed input should compile with GCC and Clang. Those
  will be used here as reference implementations.

## Requirements

- Make
- Flex
- Bison
- Clang

### How to install requirements

- Ubuntu: `sudo apt install -y make flex bison clang`
- Arch Linux: `sudo pacman -S make flex bison clang`

### Build

    make

### Test

    make test

### Tested with versions

- Make: `4.3-3` (Arch); `4.1` (Ubuntu Bionic)
- Flex: `2.6.4-3` (Arch); `2.6.4-3` (Ubuntu Bionic)
- Bison: `3.7.2-1` (Arch); `3.0.4` (Ubuntu Bionic)
- Clang: `11.0.0-1` (Arch); `6.0.0-1` (Ubuntu Bionic)

You can see the versions that are being used in your environment with the
following command:

    make show-versions
