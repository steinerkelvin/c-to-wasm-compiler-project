
(module
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

(global $fp (mut i32) (i32.const 0))
(global $sp (mut i32) (i32.const 0))

(func $fac
(param i32)
(result i32)
(get_global $sp)
(i32.const 8)
(i32.add)
(set_global $sp)
;; n
(get_global $fp)
(i32.const 4)
(i32.add)
(get_local 0)
(i32.store)
(block $b0
(get_global $fp)
(i32.const 4)
(i32.add)
(i32.load)
(i32.const 1)
(i32.le_s)
(i32.eqz)
(br_if $b0)
(i32.const 1)
(return)
)
(get_global $fp)
(i32.const 4)
(i32.add)
(i32.load)
(get_global $fp)
(i32.const 4)
(i32.add)
(i32.load)
(i32.const 1)
(i32.sub)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $fac)
(get_global $fp)
(i32.load)
(set_global $fp)
(i32.mul)
(return)
(get_global $sp)
(i32.const 8)
(i32.sub)
(set_global $sp)
)

(func $fac_iter
(param i32)
(result i32)
(get_global $sp)
(i32.const 16)
(i32.add)
(set_global $sp)
;; n
(get_global $fp)
(i32.const 4)
(i32.add)
(get_local 0)
(i32.store)
;; (ExprStmt (= "[integer]" (Var "acc" "[integer]") (Integer "[integer]" 1)))
(get_global $fp)
(i32.const 8)
(i32.add)
(i32.const 1)
(i32.store)
(i32.const 1)
(drop)
;; (ExprStmt (= "[integer]" (Var "i" "[integer]") (Integer "[integer]" 2)))
(get_global $fp)
(i32.const 12)
(i32.add)
(i32.const 2)
(i32.store)
(i32.const 2)
(drop)
(block $loopbrk0
(loop $loopaux0
(block $loop0
(get_global $fp)
(i32.const 12)
(i32.add)
(i32.load)
(get_global $fp)
(i32.const 4)
(i32.add)
(i32.load)
(i32.le_s)
(i32.eqz)
(br_if $loopbrk0)
;; (ExprStmt (= "[integer]" (Var "acc" "[integer]") (* "[integer]" (Var "acc" "[integer]") (Var "i" "[integer]"))))
(get_global $fp)
(i32.const 8)
(i32.add)
(get_global $fp)
(i32.const 8)
(i32.add)
(i32.load)
(get_global $fp)
(i32.const 12)
(i32.add)
(i32.load)
(i32.mul)
(i32.store)
(get_global $fp)
(i32.const 8)
(i32.add)
(i32.load)
(get_global $fp)
(i32.const 12)
(i32.add)
(i32.load)
(i32.mul)
(drop)
)
;; (ExprStmt (= "[integer]" (Var "i" "[integer]") (+ "[integer]" (Var "i" "[integer]") (Integer "[integer]" 1))))
(get_global $fp)
(i32.const 12)
(i32.add)
(get_global $fp)
(i32.const 12)
(i32.add)
(i32.load)
(i32.const 1)
(i32.add)
(i32.store)
(get_global $fp)
(i32.const 12)
(i32.add)
(i32.load)
(i32.const 1)
(i32.add)
(drop)
(br $loopaux0)
)
)
(get_global $fp)
(i32.const 8)
(i32.add)
(i32.load)
(return)
(get_global $sp)
(i32.const 16)
(i32.sub)
(set_global $sp)
)

(func $main
(result i32)
(get_global $sp)
(i32.const 12)
(i32.add)
(set_global $sp)
;; (ExprStmt (= "[integer]" (Var "x" "[integer]") ("f(x)" "[integer]" (Var "fac" "[(integer n)integer]") ( (Integer "[integer]" 6)))))
(get_global $fp)
(i32.const 4)
(i32.add)
(i32.const 6)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $fac)
(get_global $fp)
(i32.load)
(set_global $fp)
(i32.store)
(i32.const 6)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $fac)
(get_global $fp)
(i32.load)
(set_global $fp)
(drop)
;; (ExprStmt (= "[integer]" (Var "y" "[integer]") ("f(x)" "[integer]" (Var "fac_iter" "[(integer n)integer]") ( (Integer "[integer]" 6)))))
(get_global $fp)
(i32.const 8)
(i32.add)
(i32.const 6)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $fac_iter)
(get_global $fp)
(i32.load)
(set_global $fp)
(i32.store)
(i32.const 6)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $fac_iter)
(get_global $fp)
(i32.load)
(set_global $fp)
(drop)
;; (ExprStmt ("f(x)" "[void]" (Var "println_int" "[(integer)void]") ( (Var "x" "[integer]"))))
(get_global $fp)
(i32.const 4)
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
;; (ExprStmt ("f(x)" "[void]" (Var "println_int" "[(integer)void]") ( (Var "y" "[integer]"))))
(get_global $fp)
(i32.const 8)
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
;; (ExprStmt ("f(x)" "[void]" (Var "println_int" "[(integer)void]") ( (== "[integer]" (Var "x" "[integer]") (Var "y" "[integer]")))))
(get_global $fp)
(i32.const 4)
(i32.add)
(i32.load)
(get_global $fp)
(i32.const 8)
(i32.add)
(i32.load)
(i32.eq)
(get_global $sp)
(get_global $fp)
(i32.store)
(get_global $sp)
(set_global $fp)
(call $println_int)
(get_global $fp)
(i32.load)
(set_global $fp)
(i32.const 0)
(return)
(get_global $sp)
(i32.const 12)
(i32.sub)
(set_global $sp)
)

)
