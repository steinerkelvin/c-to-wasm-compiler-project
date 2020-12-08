const { readFileSync } = require("fs")

const ARGS = process.argv.slice(2)
const input_file_name = ARGS[0]

if (!input_file_name) {
  console.error("Missing first paramenter: input file name")
  process.exit(1)
}

const utf8_decoder = new TextDecoder('utf-8')

const make_imports = (global) => {
  const std = {
    _ln: () => { process.stdout.write("\n") },
    _print: (po, len) => {
      const { exports, memory } = global
      const arr = new Uint8Array(memory.buffer, po, len)
      const txt = utf8_decoder.decode(arr)
      process.stdout.write(txt)
    },
    _println: (po, len) => {
      std._print(po, len)
      std._ln()
    },
    print_int: (i) => {
      process.stdout.write(i.toString())
    },
    print_real: (r) => {
      process.stdout.write(r.toString())
    },
    println_int: (i) => {
      process.stdout.write(i.toString())
      std._ln()
    },
    println_real: (r) => {
      process.stdout.write(r.toString())
      std._ln()
    },
    print_int_pad: (i, full_len) => {
      let is_left = false;
      if (full_len < 0) {
        full_len = Math.abs(full_len)
        is_left = true
      }
      let txt = i.toString()
      if (is_left) {
        txt = txt.padStart(full_len)
      } else {
        txt = txt.padEnd(full_len)
      }
      process.stdout.write(txt)
    },
    print_real_pad: (r, full_len) => {
      let is_left = false
      if (full_len < 0) {
        full_len = Math.abs(full_len)
        is_left = true
      }
      let txt = i.toString()
      if (is_left) {
        txt = txt.padStart(full_len)
      } else {
        txt = txt.padEnd(full_len)
      }
      process.stdout.write(txt)
    },
  }
  return { "std": std }
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
