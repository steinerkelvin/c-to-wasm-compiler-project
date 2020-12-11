const term_opts = {
    fontSize: 16,
};

function run() {
    const term = new Terminal();
    term.open(document.getElementById("terminal"));
    term.write("Hello from \x1B[1;3;31mxterm.js\x1B[0m $ ");    
}

window.onload = run;
