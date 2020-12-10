
(module
    (import "std" "readln" (func $readln (param i32) (param i32) (result i32)))
    (import "std" "_ln" (func $_ln))
    (import "std" "_print" (func $_print (param i32) (param i32)))
    (import "std" "_println" (func $_println (param i32) (param i32)))
    (import "std" "print_int" (func $print_int (param i32)))
    (import "std" "print_real" (func $print_real (param f32)))
    (import "std" "println_int" (func $println_int (param i32)))
    (import "std" "println_real" (func $println_real (param f32)))
    (import "std" "print_int_pad" (func $print_int_pad (param i32) (param i32)))
    (import "std" "print_real_pad" (func $print_real_pad (param f32) (param i32)))

    (export "memory" (memory $mem))
    (export "str_len" (func $str_len))
    (export "str_copy" (func $str_copy))
    (export "str_end" (func $str_end))
    (export "str_cat" (func $str_cat))
    (export "main" (func $main))

    (memory $mem 1)

    (func $str_len (param $po i32) (result i32)
        (local $idx i32)
        (block $out
            (loop $loop
                local.get $po
                local.get $idx
                i32.add
                i32.load8_u
                i32.eqz
                br_if $out
                local.get $idx
                i32.const 1
                i32.add
                local.set $idx
                br $loop
            )
        )
        local.get $idx
    )
    (func $str_copy (param $dest i32) (param $src i32)
        (local $c i32)
        (block $out
            (loop $loop
                local.get $src
                i32.load8_u
                local.tee $c
                i32.eqz
                br_if $out
                local.get $dest
                local.get $c
                i32.store8
                local.get $src
                i32.const 1
                i32.add
                local.set $src
                local.get $dest
                i32.const 1
                i32.add
                local.set $dest
                br $loop
            )
        )
    )
    (func $str_end (param $po i32) (result i32)
        (block $out
            (loop $loop
                local.get $po
                i32.load8_u
                i32.eqz
                br_if $out
                local.get $po
                i32.const 1
                i32.add
                local.set $po
                br $loop
            )
        )
        local.get $po
    )
    (func $print (param $po i32)
        local.get $po
        local.get $po
        call $str_len
        call $_print
    )
    (func $println (param $po i32)
        local.get $po
        call $print
        call $_ln
    )
    (func $str_cat (param $dest i32) (param $src i32)
        local.get $dest
        call $str_end
        local.get $src
        call $str_copy
    )

(global $fp (mut i32) (i32.const 56))
(global $sp (mut i32) (i32.const 56))

(global $temp_i32 (mut i32) (i32.const 0))
(global $temp_f32 (mut f32) (f32.const 0))

(data (i32.const 0) "buffer: [")
(data (i32.const 10) "]")
(data (i32.const 12) "line size: ")
(data (i32.const 24) "did overflow: ")
(data (i32.const 39) "YES")
(data (i32.const 43) "NO, its fine")
(func $main
(result i32)
(get_global $sp)
(i32.const 16)
(i32.add)
(set_global $sp)
;; {
;; {
;; (= "[integer]" (Var "red" "[integer]") ("f(x)" "[integer]" (Var "readln" "[(*char, integer)integer]") ( (V2P "[*char]" (Var "buffer" "[[8]char]")) (Integer "[integer]" 8))))
(get_global $fp)
(i32.const 12)
(i32.add)
;; ("f(x)" "[integer]" (Var "readln" "[(*char, integer)integer]") ( (V2P "[*char]" (Var "buffer" "[[8]char]")) (Integer "[integer]" 8)))
;; (V2P "[*char]" (Var "buffer" "[[8]char]"))
(get_global $fp)
(i32.const 4)
(i32.add)
;; (Integer "[integer]" 8)
(i32.const 8)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $readln)
(get_global $fp)
(i32.load)
(set_global $fp)
(set_global $temp_i32)
(get_global $temp_i32)
(i32.store)
(get_global $temp_i32)
(drop)
;; }
;; ("f(x)" "[void]" (Var "print" "[(*char)void]") ( (String "[*char]" "0")))
;; (String "[*char]" "0")
(i32.const 0)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $print)
(get_global $fp)
(i32.load)
(set_global $fp)
;; ("f(x)" "[void]" (Var "print" "[(*char)void]") ( (V2P "[*char]" (Var "buffer" "[[8]char]"))))
;; (V2P "[*char]" (Var "buffer" "[[8]char]"))
(get_global $fp)
(i32.const 4)
(i32.add)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $print)
(get_global $fp)
(i32.load)
(set_global $fp)
;; ("f(x)" "[void]" (Var "println" "[(*char)void]") ( (String "[*char]" "1")))
;; (String "[*char]" "1")
(i32.const 10)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $println)
(get_global $fp)
(i32.load)
(set_global $fp)
;; ("f(x)" "[void]" (Var "print" "[(*char)void]") ( (String "[*char]" "2")))
;; (String "[*char]" "2")
(i32.const 12)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $print)
(get_global $fp)
(i32.load)
(set_global $fp)
;; ("f(x)" "[void]" (Var "println_int" "[(integer)void]") ( (Var "red" "[integer]")))
;; (Var "red" "[integer]")
(get_global $fp)
(i32.const 12)
(i32.add)
(i32.load)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $println_int)
(get_global $fp)
(i32.load)
(set_global $fp)
;; ("f(x)" "[void]" (Var "print" "[(*char)void]") ( (String "[*char]" "3")))
;; (String "[*char]" "3")
(i32.const 24)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $print)
(get_global $fp)
(i32.load)
(set_global $fp)
(block $b0
;; (> "[integer]" (Var "red" "[integer]") (Integer "[integer]" 8))
;; (Var "red" "[integer]")
(get_global $fp)
(i32.const 12)
(i32.add)
(i32.load)
;; (Integer "[integer]" 8)
(i32.const 8)
(i32.gt_s)
(i32.eqz)
(br_if $b0)
;; {
;; ("f(x)" "[void]" (Var "println" "[(*char)void]") ( (String "[*char]" "4")))
;; (String "[*char]" "4")
(i32.const 39)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $println)
(get_global $fp)
(i32.load)
(set_global $fp)
;; (Integer "[integer]" 0)
(i32.const 0)
(return)
;; }
)
;; ("f(x)" "[void]" (Var "println" "[(*char)void]") ( (String "[*char]" "5")))
;; (String "[*char]" "5")
(i32.const 43)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $println)
(get_global $fp)
(i32.load)
(set_global $fp)
;; (Integer "[integer]" 0)
(i32.const 0)
(return)
;; }
(get_global $sp)
(i32.const 16)
(i32.sub)
(set_global $sp)
)

)
