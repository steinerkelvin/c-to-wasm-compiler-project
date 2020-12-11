const term_opts = {
    fontSize: 18,
    rows: 32,
    columns: 32,
};

const delay = 200;

const make_imports = (global) => {
    const utf8_decoder = new TextDecoder('utf-8')
    const utf8_encoder = new TextEncoder('utf-8')

    function readline(terminal) {
        return "q";
    }

    const std = {
        sleep: (ms) => {
            // const start = new Date().getTime()
            // const expire = start + ms
            // while (new Date().getTime() < expire) { }
            // return
        },
        readln: (po, buf_len) => {
            const { memory, terminal } = global
            const line = readline(terminal)
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
        _ln: () => {
            const { terminal } = global
            console.log("ln")
            terminal.writeln("")
        },
        _print: (po, len) => {
            const { memory, terminal } = global
            const arr = new Uint8Array(memory.buffer, po, len)
            const txt = utf8_decoder.decode(arr)
            terminal.write(txt)
        },
        _println: (po, len) => {
            std._print(po, len)
            std._ln()
        },
        print_int: (i) => {
            const { terminal } = global
            terminal.write(txt)
        },
        print_real: (r) => {
            const { terminal } = global
            terminal.write(txt)
        },
        println_int: (i) => {
            const { terminal } = global
            terminal.write(txt)
            std._ln()
        },
        println_real: (r) => {
            const { terminal } = global
            terminal.write(txt)
            std._ln()
        },
        print_int_pad: (i, full_len) => {
            console.log("print_int_pad")
            const { terminal } = global
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
            terminal.write(txt)
        },
        print_real_pad: (r, full_len) => {
            const { terminal } = global
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
            terminal.write(txt)
        },
    }
    return { "std": std }
}

const run_wasm = async (source_buffer, { terminal }) => {
    const module = await WebAssembly.compile(source_buffer)

    const global = {
        terminal,
        exports: null,
        memory: null,
    }

    const imports = make_imports(global)

    const instance = await WebAssembly.instantiate(module, imports)

    const exports = instance.exports
    // console.error(exports)  // DEBUG

    if (!exports.memory) {
        console.error("wasm program does not export memory")
        return
    }

    global.exports = exports
    global.memory = exports.memory

    if (exports.start && exports.loop) {
        exports.start()
        function run_loop() {
            let result = exports.loop()
            if (result) {
                setTimeout(run_loop, delay);
            }
        }
        setTimeout(run_loop, 0)
    } else if (exports.loop) {
        exports.main()
    } else {
        console.error("wasm program does not export `main` or `loop` function")
        return
    }
}

const run = async () => {
    const source_file = "app/life.wasm"

    const terminal = new Terminal(term_opts)
    terminal.open(document.getElementById("terminal"))
    terminal.write("Running ")
    terminal.write("\x1B[1;3;31m")
    terminal.write(`${source_file}`)
    terminal.write("\x1B[0m")
    terminal.writeln(" ...")

    const source_buffer =
        await fetch(source_file)
            .then(response => response.arrayBuffer())

    run_wasm(source_buffer, { terminal }).catch((err) => {
        if (err) {
            console.error(err)
            process.exit(1)
        }
    })
}

window.onload = () => {
    run()
        .catch(err => console.log(err))
};
