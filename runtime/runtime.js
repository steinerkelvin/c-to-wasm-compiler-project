const { readFileSync } = require("fs")
const util = require('util')
const readlineSync = require("./readline-sync")

const make_imports = (global) => {
  const utf8_decoder = new util.TextDecoder('utf-8')
  const utf8_encoder = new util.TextEncoder('utf-8')

  const std = {
    sleep: (ms) => {
      const start = new Date().getTime()
      const expire = start + ms
      while (new Date().getTime() < expire) { }
      return
    },
    readln: (po, buf_len) => {
      const { memory } = global
      const line = readlineSync.question('')
      const in_arr = utf8_encoder.encode(line)
      const in_len = in_arr.length
      const out_arr = new Uint8Array(memory.buffer, po, buf_len)
      const overflowed = in_len >= buf_len
      const str_len = overflowed ? buf_len - 1 : in_len
      for (let i = 0; i < str_len; i++) {
        out_arr[i] = in_arr[i]
      }
      out_arr[str_len] = 0;
      return in_len
    },
    _ln: () => { process.stdout.write("\n") },
    _print: (po, len) => {
      const { memory } = global
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

const run_wasm = async (source_buffer) => {
  const module = await WebAssembly.compile(source_buffer)

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

const run = () => {
  const ARGS = process.argv.slice(2)
  const source_file_name = ARGS[0]

  if (!source_file_name) {
    console.error("Missing first paramenter: input file name")
    process.exit(1)
  }

  const source_buffer = readFileSync(source_file_name)

  run_wasm(source_buffer).catch((err) => {
    if (err) {
      console.error(err)
      process.exit(1)
    }
  })
}

run()
