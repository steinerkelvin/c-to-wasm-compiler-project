const { readFileSync } = require("fs")

const ARGS = process.argv.slice(2)
const input_file_name = ARGS[0]

if (!input_file_name) {
  console.error("Missing first paramenter: input file name")
  process.exit(1)
}

const run = async (file_name) => {
  const program_buffer = readFileSync(file_name)
  const module = await WebAssembly.compile(program_buffer)

  const global_stuff = {
    memory: null
  }

  const env = {
    runtime: {
      println: (po) => {
        // const sub = global_stuff.memory.subarray(po)
        console.log(`printing string at position ${po}`)
      },
      println_int: (i) => console.log(i),
      println_real: (r) => console.log(r),
    }
  }

  const instance = await WebAssembly.instantiate(module, env)

  const exports = instance.exports
  // console.log(exports)

  if (!exports.memory) {
    console.error("wasm program does not export memory")
    process.exit(1)
  }
  if (!exports.main) {
    console.error("wasm program does not export main function")
    process.exit(1)
  }

  global_stuff.memory = exports.memory
  exports.main()
}

run(input_file_name).catch((err) => {
  if (err) {
    console.error(err)
  }
})
