
exe="./exe"

all: exe
	@echo "Done."

bison: src/parser.c src/parser.h

flex: src/scanner.c


src/parser.c src/parser.h: src/parser.y
	(cd src/; bison parser.y)

src/scanner.c: src/scanner.l src/parser.h
	(cd src/; flex scanner.l)

exe: src/scanner.c src/parser.c
	gcc -Wall -o ${exe} src/*.c


clean:
	(cd src/; rm -f *.o *.output scanner.c parser.h parser.c ./exe)
