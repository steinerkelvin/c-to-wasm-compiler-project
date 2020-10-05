cc=clang++
flags=-Wall

all: exe

test: exe
	./run_tests.sh

flex: src/scanner.cpp

bison: src/parser.cpp src/parser.h

exe: bin/compiler bin/dump-tokens

%/:
	mkdir -p $@

src/generated_parser.cpp src/generated_parser.h: src/parser.y
	(cd src/; bison -v "parser.y")

src/scanner.cpp: src/scanner.l src/generated_parser.h
	(cd src/; flex "scanner.l")

bin/compiler: src/scanner.cpp src/generated_parser.cpp src/symtable.cpp src/main.cpp | bin/
	$(cc) $(flags) -o "$@" $^ ${flags}

bin/dump-tokens: src/scanner.cpp src/dump_tokens.cpp | bin/
	$(cc) $(flags) -o "$@" $^ ${flags} -D DUMP_TOKENS

clean:
	rm -f bin/*
	(cd src/; rm -f *.output generated_parser.h generated_parser.cpp scanner.cpp)
