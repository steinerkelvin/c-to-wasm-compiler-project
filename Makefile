
cc=clang++
flags=-Wall
exe="./exe"

all: exe
	@echo "Done."

bison: src/parser.cpp src/parser.h

flex: src/scanner.cpp


src/generated_parser.cpp src/generated_parser.h: src/parser.y
	(cd src/; bison -v parser.y)

src/scanner.cpp: src/scanner.l src/generated_parser.h
	(cd src/; flex scanner.l)

exe: src/scanner.cpp src/generated_parser.cpp src/symtable.cpp src/main.cpp
	$(cc) $(flags) -o ${exe} $^ ${flags}


clean:
	rm -f ./exe
	(cd src/; rm -f *.output generated_parser.h generated_parser.cpp scanner.cpp)
