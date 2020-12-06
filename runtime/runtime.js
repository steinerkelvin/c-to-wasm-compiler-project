const { readFileSync } = require("fs")

const ARGS = process.argv.slice(2)
const input_file_name = ARGS[0]

if (!input_file_name) {
  console.error("Missing first paramenter: input file name")
  process.exit(1)
}

const utf8_decoder = new TextDecoder('utf-8')

const make_imports = (global) => {
  return {
    "std": {
      println: (po) => {
        const {exports, memory} = global
        // TODO do without wasm-implementd str_len ??
        const len = exports.str_len(po)
        const arr = new Uint8Array(memory.buffer, po, len)
        const txt = utf8_decoder.decode(arr)
        console.log(txt)
      },
      println_int: (i) => console.log(i),
      println_real: (r) => console.log(r),
    }
  }
}

const run = async (file_name) => {
  const program_buffer = readFileSync(file_name)
  const module = await WebAssembly.compile(program_buffer)

  const global = {
    exports: null,
    memory: null,
  }

  const imports = make_imports(global)

  const instance = await WebAssembly.instantiate(module, imports)

  const exports = instance.exports
  // console.error(exports)  // DEBUG

  if (!exports.memory) {
    console.error("wasm program does not export memory")
    process.exit(1)
  }
  if (!exports.main) {
    console.error("wasm program does not export main function")
    process.exit(1)
  }

  global.exports = exports
  global.memory = exports.memory

  exports.main()
}

run(input_file_name).catch((err) => {
  if (err) {
    console.error(err)
    process.exit(1)
  }
})
