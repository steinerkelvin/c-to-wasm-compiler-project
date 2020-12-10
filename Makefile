CXX=clang++
CXXFLAGS=-std=c++17 -Wall -g

parser_files = src/generated_parser.cpp src/generated_parser.hpp src/generated_parser.output
scanner_files = src/scanner.cpp
generated_files = $(parser_files) $(scanner_files)
generated_headers = src/generated_parser.hpp
generated_srcs = src/scanner.cpp src/generated_parser.cpp
generated_objs = $(patsubst src/%.cpp,build/%.o,$(generated_srcs))

# Arquivos de bibliotecas
src_lib=$(wildcard src/*.cpp)
objs=$(sort $(patsubst src/%.cpp,build/%.o,$(src_lib)) $(generated_objs) )

# Arquivos de executáveis
src_main=$(wildcard main/*.cpp)
mains=$(patsubst main/%.cpp,bin/%,$(src_main))

src_custom=$(wildcard custom/*.cpp)
customs=$(patsubst custom/%.cpp,bin/custom/%,$(src_custom))

all: exe

# TODO separate dependencies on binaries
test-parsing: exe
	@./run_parsing_tests.sh

render-asts: exe
	@./run_render_asts.sh

test-codegen: exe
	@./run_codegen_tests.sh

format:
	clang-format -i src/**.cpp src/**.hpp

doxygen:
	doxygen

cp=cp2

pdf:
	pandoc ./relatorio-dope-$(cp).md -o ./relatorio-dope-$(cp).pdf

name="compiladores-dope-$(cp)"
zip:
	@# zip -r compiladores-dope-$(cp).zip ./ -x '.git/*' 'tmp/*' 'doxy/*' '.dep/*'
	git archive --prefix=$(name)/ -o $(name).zip HEAD

clean:
	rm -f -r ./.dep
	rm -f -r ./bin
	rm -f -r ./build
	rm -f -r ./doxy
	rm -f $(generated_files)

show-versions:
	make --version
	@echo
	flex --version
	@echo
	bison --version
	@echo
	clang --version
	@echo
	node --version
	@echo

flex: $(scanner_files)

bison: $(parser_files)

exe: $(mains) $(customs)

# bin/dump-tokens: src/scanner.cpp custom/dump-tokens.cpp
# 	mkdir -p ./bin
# 	$(CXX) $(CXXFLAGS) -o "$@" $^ -D DUMP_TOKENS

$(parser_files): src/parser.y
	cd src/; bison -v "parser.y"

$(scanner_files): src/scanner.l src/generated_parser.hpp
	cd src/; flex "scanner.l"


$(objs) $(customs): $(generated_headers)
$(mains): $(objs)

# Compila bibliotecas de src/ para build/
build/%.o: src/%.cpp
	@ mkdir -p build/ $(dir .dep/src/%)
	$(CXX) $(CXXFLAGS) -o $@  -c $<
	@ $(CXX) $(CXXFLAGS) -MM -MT $@  $< > .dep/$<.d

# Compila de main/ para para bin/
bin/%: main/%.cpp $(objs)
	@ mkdir -p bin/ build/main/ $(dir ./.dep/main/%)
	$(CXX) $(CXXFLAGS) -I "./src/" -c -o build/main/$*.o $<
	$(CXX) $(CXXFLAGS) -I "./src/" -o $@ build/main/$*.o $(objs)
	@ $(CXX) $(CXXFLAGS) -I "./src/" -MM -MT $@ $<  > .dep/$<.d

# Compila de main/ para para bin/
bin/custom/%: custom/%.cpp
	@ mkdir -p bin/custom/ $(dir ./.dep/custom/%)
	$(CXX) $(CXXFLAGS) -I "./src/" -o $@ $<
	@ $(CXX) $(CXXFLAGS) -I "./src/" -MM -MT $@ $<  > .dep/$<.d

# Inclui as listas de dependências
include $(wildcard .dep/**/*)
