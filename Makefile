
cc=clang++
flags=-Wall
exe="./exe"

all: exe
	@echo "Done."

bison: src/parser.cpp src/parser.h

flex: src/scanner.cpp


src/parser.cpp src/parser.h: src/parser.y
	(cd src/; bison -v parser.y)

src/scanner.cpp: src/scanner.l src/parser.h
	(cd src/; flex scanner.l)

exe: src/scanner.cpp src/parser.cpp
	$(cc) $(flags) -o ${exe} $^ ${flags}


clean:
	rm -f ./exe
	(cd src/; rm -f *.output parser.h parser.cpp scanner.cpp)
