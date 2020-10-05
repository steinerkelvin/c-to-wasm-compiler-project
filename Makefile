cc=clang++
flags=-Wall

all: bin/compiler

test: all
	./run_tests.sh

bison: src/parser.cpp src/parser.h

flex: src/scanner.cpp


%/:
	mkdir -p $@

src/generated_parser.cpp src/generated_parser.h: src/parser.y
	(cd src/; bison -v "parser.y")

src/scanner.cpp: src/scanner.l src/generated_parser.h
	(cd src/; flex "scanner.l")

bin/compiler: src/scanner.cpp src/generated_parser.cpp src/symtable.cpp src/main.cpp | bin/
	$(cc) $(flags) -o "$@" $^ ${flags}


clean:
	rm -f bin/*
	(cd src/; rm -f *.output generated_parser.h generated_parser.cpp scanner.cpp)
