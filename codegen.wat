
(module
    (import "std" "println" (func $println (param i32)))
    (import "std" "println_int" (func $println_int (param i32)))
    (import "std" "println_real" (func $println_real (param f32)))

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
    (func $str_cat (param $dest i32) (param $src i32)
        local.get $dest
        call $str_end
        local.get $src
        call $str_copy
    )

(global $fp i32 (i32.const 0))
(global $sp i32 (i32.const 0))

(func $main
;; (ExprStmt (= "[integer]" (Var "global" "[integer]") (Integer "[integer]" 1)))
(i32.const 0)
(i32.const 1)
(i32.store)
(i32.const 1)
(drop)
;; (ExprStmt (= "[integer]" (Var "x" "[integer]") (Integer "[integer]" 1)))
(i32.const 4)
(get_global $fp)
(i32.add)
(i32.const 1)
(i32.store)
(i32.const 1)
(drop)
;; (ExprStmt (= "[integer]" (Var "y" "[integer]") (Integer "[integer]" 2)))
(i32.const 8)
(get_global $fp)
(i32.add)
(i32.const 2)
(i32.store)
(i32.const 2)
(drop)
;; (ExprStmt (= "[integer]" (Var "global" "[integer]") (+ "[integer]" (Var "x" "[integer]") (Var "y" "[integer]"))))
(i32.const 0)
(i32.const 4)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 8)
(get_global $fp)
(i32.add)
(i32.load)
(i32.add)
(i32.store)
(i32.const 4)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 8)
(get_global $fp)
(i32.add)
(i32.load)
(i32.add)
(drop)
;; (ExprStmt ("f(x)" "[void]" (Var "println_int" "[(integer)void]") ( (Var "global" "[integer]"))))
(i32.const 0)
(i32.load)
(call $println_int)
;; (ExprStmt (= "[real]" (Var "r" "[real]") (Real "[real]" 4.2)))
(i32.const 12)
(get_global $fp)
(i32.add)
(f32.const 4.2)
(f32.store)
(f32.const 4.2)
(drop)
(block $b0
(i32.const 0)
(i32.load)
(i32.eqz)
(br_if $b0)
;; (ExprStmt (= "[real]" (Var "r" "[real]") (Real "[real]" 77.7)))
(i32.const 12)
(get_global $fp)
(i32.add)
(f32.const 77.7)
(f32.store)
(f32.const 77.7)
(drop)
)
;; (ExprStmt ("f(x)" "[void]" (Var "println_real" "[(real)void]") ( (Var "r" "[real]"))))
(i32.const 12)
(get_global $fp)
(i32.add)
(f32.load)
(call $println_real)
;; (ExprStmt (= "[integer]" (Var "num" "[integer]") (Integer "[integer]" 12)))
(i32.const 16)
(get_global $fp)
(i32.add)
(i32.const 12)
(i32.store)
(i32.const 12)
(drop)
(block $loopbrk0
(loop $loop0
(i32.const 16)
(get_global $fp)
(i32.add)
(i32.load)
(i32.eqz)
(br_if $loopbrk0)
;; (ExprStmt (= "[integer]" (Var "num" "[integer]") (- "[integer]" (Var "num" "[integer]") (Integer "[integer]" 1))))
(i32.const 16)
(get_global $fp)
(i32.add)
(i32.const 16)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 1)
(i32.sub)
(i32.store)
(i32.const 16)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 1)
(i32.sub)
(drop)
(block $b1
(i32.const 16)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 2)
(i32.rem_s)
(i32.eqz)
(br_if $b1)
(br $loop0)
)
(block $b2
(i32.const 16)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 4)
(i32.eq)
(i32.eqz)
(br_if $b2)
(block $loopbrk1
(loop $loop1
(i32.const 1)
(i32.eqz)
(br_if $loopbrk1)
(br $loopbrk1)
(br $loop1)
)
)
;; (ExprStmt ("f(x)" "[void]" (Var "println_int" "[(integer)void]") ( (Integer "[integer]" 777))))
(i32.const 777)
(call $println_int)
(br $loopbrk0)
)
;; (ExprStmt ("f(x)" "[void]" (Var "println_int" "[(integer)void]") ( (Var "num" "[integer]"))))
(i32.const 16)
(get_global $fp)
(i32.add)
(i32.load)
(call $println_int)
(br $loop0)
)
)
;; (ExprStmt (= "[integer]" (Var "i" "[integer]") (Integer "[integer]" 0)))
(i32.const 20)
(get_global $fp)
(i32.add)
(i32.const 0)
(i32.store)
(i32.const 0)
(drop)
(block $loopbrk0
(loop $loopaux0
(block $loop0
(block $b3
(i32.const 20)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 3)
(i32.rem_s)
(i32.const 0)
(i32.eq)
(i32.eqz)
(br_if $b3)
(br $loop0)
)
;; (ExprStmt ("f(x)" "[void]" (Var "println_int" "[(integer)void]") ( (Var "i" "[integer]"))))
(i32.const 20)
(get_global $fp)
(i32.add)
(i32.load)
(call $println_int)
(block $b4
(i32.const 20)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 10)
(i32.eq)
(i32.eqz)
(br_if $b4)
(br $loopbrk0)
)
)
;; (ExprStmt (= "[integer]" (Var "i" "[integer]") (+ "[integer]" (Var "i" "[integer]") (Integer "[integer]" 1))))
(i32.const 20)
(get_global $fp)
(i32.add)
(i32.const 20)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 1)
(i32.add)
(i32.store)
(i32.const 20)
(get_global $fp)
(i32.add)
(i32.load)
(i32.const 1)
(i32.add)
(drop)
(br $loopaux0)
)
)
)

)
