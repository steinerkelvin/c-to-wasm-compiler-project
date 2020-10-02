
name=hello

all: ${name}.wasm

wat: ${name}.wat

run: ${name}.wasm
	wasmer run "${name}.wasm"

${name}.o: ${name}.c
	clang --target=wasm32 -O0 -g -c -o "$@" "$^"
	
${name}.wasm: ${name}.o
	wasm-ld --no-entry --strip-all --export-all --allow-undefined -o "$@" "$^" 

${name}.wat: ${name}.wasm
	wasm2wat "$<" > "$@"

clean:
	rm -f ${name}.o ${name}.wasm ${name}.wat
