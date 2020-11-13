# compiladores-trabalho

Hopefully, a C to WebAssemply compiler.

## Guidelines

- Os arquivos de teste com entradas bem formadas devem compilar com o GCC e o
  Clang, utilizados aqui como implementações de referência.

## Requisitos para compilação

- Make
- Flex
- Bison
- Clang

### Como installar requisitos

- Ubuntu: `sudo apt install make flex bison clang -y`
- Arch Linux: `sudo pacman -S make flex bison clang`

### Testado com as versões

- Make: `4.3-3` (Arch); `4.1` (Ubuntu Bionic)
- Flex: `2.6.4-3` (Arch); `2.6.4-3` (Ubuntu Bionic)
- Bison: `3.7.2-1` (Arch); `3.0.4` (Ubuntu Bionic)
- Clang: `11.0.0-1` (Arch); `6.0.0-1` (Ubuntu Bionic)
