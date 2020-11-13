cc=clang++ -std=c++17
cflags=-Wall -g

parser_files = src/generated_parser.cpp src/generated_parser.hpp src/generated_parser.output
scanner_files = src/scanner.cpp
generated_files = $(parser_files) $(scanner_files)
generated_headers = src/generated_parser.hpp

# Arquivos de bibliotecas
src_lib=$(wildcard src/*.cpp)
objs=$(patsubst src/%.cpp,build/%.o,$(src_lib))

# Arquivos de executáveis
src_main=$(wildcard main/*.cpp)
mains=$(patsubst main/%.cpp,bin/%,$(src_main))

src_custom=$(wildcard custom/*.cpp)
customs=$(patsubst custom/%.cpp,bin/custom/%,$(src_custom))

all: exe

test: exe
	@ echo TESTING
	./run_tests.sh

format:
	clang-format -i src/**.cpp src/**.hpp

doxygen:
	doxygen

pdf:
	pandoc ./relatorio-cp2.md -o ./relatorio-cp2.pdf

clean:
	rm -f -r ./.deps
	rm -f -r ./bin
	rm -f -r ./build
	cd src/; rm -f *.output $(parser_files) $(scanner_files)

show-versions:
	make --version ; echo
	flex --version ; echo
	bison --version ; echo
	clang --version ; echo


flex: $(scanner_files)

bison: $(parser_files)

exe: $(mains) $(customs)

# bin/dump-tokens: src/scanner.cpp custom/dump-tokens.cpp
# 	mkdir -p ./bin
# 	$(cc) $(cflags) -o "$@" $^ $(cflags) -D DUMP_TOKENS

$(parser_files): src/parser.y
	cd src/; bison -v "parser.y"

$(scanner_files): src/scanner.l src/generated_parser.hpp
	cd src/; flex "scanner.l"


$(objs) $(customs): $(generated_headers)
$(mains): $(objs)

# Compila bibliotecas de src/ para build/
build/%.o: src/%.cpp
	@ mkdir -p build/ $(dir .dep/src/%)
	$(cc) $(cflags) -o $@  -c $<
	@ $(cc) $(cflags) -MM -MT $@  $< > .dep/$<.d

# Compila de main/ para para bin/
bin/%: main/%.cpp $(objs)
	@ mkdir -p bin/ build/main/ $(dir ./.dep/main/%)
	$(cc) $(cflags) -I "./src/" -c -o build/main/$*.o $<
	$(cc) $(cflags) -I "./src/" -o $@ build/main/$*.o $(objs)
	@ $(cc) $(cflags) -I "./src/" -MM -MT $@ $<  > .dep/$<.d

# Compila de main/ para para bin/
bin/custom/%: custom/%.cpp
	@ mkdir -p bin/custom/ $(dir ./.dep/custom/%)
	$(cc) $(cflags) -I "./src/" -o $@ $<
	@ $(cc) $(cflags) -I "./src/" -MM -MT $@ $<  > .dep/$<.d

# Inclui as listas de dependências
include $(wildcard .dep/**/*)
