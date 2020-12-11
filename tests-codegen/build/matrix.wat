
(module
    (import "std" "sleep" (func $sleep (param i32)))
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

(memory $mem 16)
(global $fp (mut i32) (i32.const 8))
(global $sp (mut i32) (i32.const 8))

(global $temp_i32 (mut i32) (i32.const 0))
(global $temp_f32 (mut f32) (f32.const 0))

(data (i32.const 0) " | ")
  (func $main
  (result i32)
  (get_global $sp)
  (i32.const 84)
  (i32.add)
  (set_global $sp)
  ;; (Block 1 (Block 2 (ForStmt (ExprStmt (= "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "i" "2,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1)))) (Block 3 (ForStmt (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1)))) (Block 4 (Block (ExprStmt (= "[integer]" (Var "ii" "4,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1))))) (Block (ExprStmt (= "[integer]" (Var "jj" "4,1" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1))))) (ExprStmt (= "[integer]" (v[x] "[integer]" (Var "j" "3,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "2,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (+ "[integer]" (* "[integer]" (Integer "[integer]" 1000) (Var "ii" "4,0" "[integer]")) (* "[integer]" (Var "jj" "4,1" "[integer]") (Var "jj" "4,1" "[integer]")))))))))) (Block 5 (ForStmt (ExprStmt (= "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "i" "5,0" "[integer]") (+ "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 1)))) (Block 6 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (Var "i" "5,0" "[integer]") (Integer "[integer]" 2)))) (ExprStmt ("f(x)" "[void]" (Var "print" "0,1" "[(*char)void]") ( (String "[*char]" "0")))) (ForStmt (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (+ "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 1)))) (Block 7 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (v[x] "[integer]" (Var "j" "6,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "5,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (Integer "[integer]" 8)))))) (ExprStmt ("f(x)" "[void]" (Var "_ln" "0,0" "[()void]") ()))))) (return (Integer "[integer]" 0)))
  ;; {
    ;; (Block 2 (ForStmt (ExprStmt (= "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "i" "2,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1)))) (Block 3 (ForStmt (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1)))) (Block 4 (Block (ExprStmt (= "[integer]" (Var "ii" "4,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1))))) (Block (ExprStmt (= "[integer]" (Var "jj" "4,1" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1))))) (ExprStmt (= "[integer]" (v[x] "[integer]" (Var "j" "3,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "2,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (+ "[integer]" (* "[integer]" (Integer "[integer]" 1000) (Var "ii" "4,0" "[integer]")) (* "[integer]" (Var "jj" "4,1" "[integer]") (Var "jj" "4,1" "[integer]"))))))))))
    ;; {
      ;; (ForStmt (ExprStmt (= "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "i" "2,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1)))) (Block 3 (ForStmt (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1)))) (Block 4 (Block (ExprStmt (= "[integer]" (Var "ii" "4,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1))))) (Block (ExprStmt (= "[integer]" (Var "jj" "4,1" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1))))) (ExprStmt (= "[integer]" (v[x] "[integer]" (Var "j" "3,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "2,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (+ "[integer]" (* "[integer]" (Integer "[integer]" 1000) (Var "ii" "4,0" "[integer]")) (* "[integer]" (Var "jj" "4,1" "[integer]") (Var "jj" "4,1" "[integer]")))))))))
      ;; (ExprStmt (= "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 0)))
      (get_global $fp)
      (i32.const 68)
      (i32.add)
      (i32.const 0)
      (set_global $temp_i32)
      (get_global $temp_i32)
      (i32.store)
      (get_global $temp_i32)
      (drop)
      (block $loopbrk0
      (loop $loopaux0
      (block $loop0
      (get_global $fp)
      (i32.const 68)
      (i32.add)
      (i32.load)
      (i32.const 4)
      (i32.lt_s)
      (i32.eqz)
      (br_if $loopbrk0)
      ;; (Block 3 (ForStmt (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1)))) (Block 4 (Block (ExprStmt (= "[integer]" (Var "ii" "4,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1))))) (Block (ExprStmt (= "[integer]" (Var "jj" "4,1" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1))))) (ExprStmt (= "[integer]" (v[x] "[integer]" (Var "j" "3,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "2,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (+ "[integer]" (* "[integer]" (Integer "[integer]" 1000) (Var "ii" "4,0" "[integer]")) (* "[integer]" (Var "jj" "4,1" "[integer]") (Var "jj" "4,1" "[integer]"))))))))
      ;; {
        ;; (ForStmt (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1)))) (Block 4 (Block (ExprStmt (= "[integer]" (Var "ii" "4,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1))))) (Block (ExprStmt (= "[integer]" (Var "jj" "4,1" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1))))) (ExprStmt (= "[integer]" (v[x] "[integer]" (Var "j" "3,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "2,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (+ "[integer]" (* "[integer]" (Integer "[integer]" 1000) (Var "ii" "4,0" "[integer]")) (* "[integer]" (Var "jj" "4,1" "[integer]") (Var "jj" "4,1" "[integer]")))))))
        ;; (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 0)))
        (get_global $fp)
        (i32.const 72)
        (i32.add)
        (i32.const 0)
        (set_global $temp_i32)
        (get_global $temp_i32)
        (i32.store)
        (get_global $temp_i32)
        (drop)
        (block $loopbrk1
        (loop $loopaux1
        (block $loop1
        (get_global $fp)
        (i32.const 72)
        (i32.add)
        (i32.load)
        (i32.const 4)
        (i32.lt_s)
        (i32.eqz)
        (br_if $loopbrk1)
        ;; (Block 4 (Block (ExprStmt (= "[integer]" (Var "ii" "4,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1))))) (Block (ExprStmt (= "[integer]" (Var "jj" "4,1" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1))))) (ExprStmt (= "[integer]" (v[x] "[integer]" (Var "j" "3,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "2,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (+ "[integer]" (* "[integer]" (Integer "[integer]" 1000) (Var "ii" "4,0" "[integer]")) (* "[integer]" (Var "jj" "4,1" "[integer]") (Var "jj" "4,1" "[integer]"))))))
        ;; {
          ;; (Block (ExprStmt (= "[integer]" (Var "ii" "4,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1)))))
          ;; {
            ;; (ExprStmt (= "[integer]" (Var "ii" "4,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1))))
            (get_global $fp)
            (i32.const 76)
            (i32.add)
            (get_global $fp)
            (i32.const 68)
            (i32.add)
            (i32.load)
            (i32.const 1)
            (i32.add)
            (set_global $temp_i32)
            (get_global $temp_i32)
            (i32.store)
            (get_global $temp_i32)
            (drop)
          ;; }
          ;; (Block (ExprStmt (= "[integer]" (Var "jj" "4,1" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1)))))
          ;; {
            ;; (ExprStmt (= "[integer]" (Var "jj" "4,1" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1))))
            (get_global $fp)
            (i32.const 80)
            (i32.add)
            (get_global $fp)
            (i32.const 72)
            (i32.add)
            (i32.load)
            (i32.const 1)
            (i32.add)
            (set_global $temp_i32)
            (get_global $temp_i32)
            (i32.store)
            (get_global $temp_i32)
            (drop)
          ;; }
          ;; (ExprStmt (= "[integer]" (v[x] "[integer]" (Var "j" "3,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "2,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (+ "[integer]" (* "[integer]" (Integer "[integer]" 1000) (Var "ii" "4,0" "[integer]")) (* "[integer]" (Var "jj" "4,1" "[integer]") (Var "jj" "4,1" "[integer]")))))
          (get_global $fp)
          (i32.const 4)
          (i32.add)
          (i32.const 16)
          (get_global $fp)
          (i32.const 68)
          (i32.add)
          (i32.load)
          (i32.mul)
          (i32.add)
          (i32.const 4)
          (get_global $fp)
          (i32.const 72)
          (i32.add)
          (i32.load)
          (i32.mul)
          (i32.add)
          (i32.const 1000)
          (get_global $fp)
          (i32.const 76)
          (i32.add)
          (i32.load)
          (i32.mul)
          (get_global $fp)
          (i32.const 80)
          (i32.add)
          (i32.load)
          (get_global $fp)
          (i32.const 80)
          (i32.add)
          (i32.load)
          (i32.mul)
          (i32.add)
          (set_global $temp_i32)
          (get_global $temp_i32)
          (i32.store)
          (get_global $temp_i32)
          (drop)
        ;; }
        )
        ;; (ExprStmt (= "[integer]" (Var "j" "3,0" "[integer]") (+ "[integer]" (Var "j" "3,0" "[integer]") (Integer "[integer]" 1))))
        (get_global $fp)
        (i32.const 72)
        (i32.add)
        (get_global $fp)
        (i32.const 72)
        (i32.add)
        (i32.load)
        (i32.const 1)
        (i32.add)
        (set_global $temp_i32)
        (get_global $temp_i32)
        (i32.store)
        (get_global $temp_i32)
        (drop)
        (br $loopaux1)
        )
        )
      ;; }
      )
      ;; (ExprStmt (= "[integer]" (Var "i" "2,0" "[integer]") (+ "[integer]" (Var "i" "2,0" "[integer]") (Integer "[integer]" 1))))
      (get_global $fp)
      (i32.const 68)
      (i32.add)
      (get_global $fp)
      (i32.const 68)
      (i32.add)
      (i32.load)
      (i32.const 1)
      (i32.add)
      (set_global $temp_i32)
      (get_global $temp_i32)
      (i32.store)
      (get_global $temp_i32)
      (drop)
      (br $loopaux0)
      )
      )
    ;; }
    ;; (Block 5 (ForStmt (ExprStmt (= "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "i" "5,0" "[integer]") (+ "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 1)))) (Block 6 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (Var "i" "5,0" "[integer]") (Integer "[integer]" 2)))) (ExprStmt ("f(x)" "[void]" (Var "print" "0,1" "[(*char)void]") ( (String "[*char]" "0")))) (ForStmt (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (+ "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 1)))) (Block 7 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (v[x] "[integer]" (Var "j" "6,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "5,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (Integer "[integer]" 8)))))) (ExprStmt ("f(x)" "[void]" (Var "_ln" "0,0" "[()void]") ())))))
    ;; {
      ;; (ForStmt (ExprStmt (= "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "i" "5,0" "[integer]") (+ "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 1)))) (Block 6 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (Var "i" "5,0" "[integer]") (Integer "[integer]" 2)))) (ExprStmt ("f(x)" "[void]" (Var "print" "0,1" "[(*char)void]") ( (String "[*char]" "0")))) (ForStmt (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (+ "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 1)))) (Block 7 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (v[x] "[integer]" (Var "j" "6,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "5,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (Integer "[integer]" 8)))))) (ExprStmt ("f(x)" "[void]" (Var "_ln" "0,0" "[()void]") ()))))
      ;; (ExprStmt (= "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 0)))
      (get_global $fp)
      (i32.const 68)
      (i32.add)
      (i32.const 0)
      (set_global $temp_i32)
      (get_global $temp_i32)
      (i32.store)
      (get_global $temp_i32)
      (drop)
      (block $loopbrk0
      (loop $loopaux0
      (block $loop0
      (get_global $fp)
      (i32.const 68)
      (i32.add)
      (i32.load)
      (i32.const 4)
      (i32.lt_s)
      (i32.eqz)
      (br_if $loopbrk0)
      ;; (Block 6 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (Var "i" "5,0" "[integer]") (Integer "[integer]" 2)))) (ExprStmt ("f(x)" "[void]" (Var "print" "0,1" "[(*char)void]") ( (String "[*char]" "0")))) (ForStmt (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (+ "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 1)))) (Block 7 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (v[x] "[integer]" (Var "j" "6,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "5,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (Integer "[integer]" 8)))))) (ExprStmt ("f(x)" "[void]" (Var "_ln" "0,0" "[()void]") ())))
      ;; {
        ;; (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (Var "i" "5,0" "[integer]") (Integer "[integer]" 2))))
        (get_global $fp)
        (i32.const 68)
        (i32.add)
        (i32.load)
        (i32.const 2)
        (get_global $sp)
        (get_global $fp)
        (i32.store)
        (get_global $sp)
        (set_global $fp)
        (call $print_int_pad)
        (get_global $fp)
        (get_global $fp)
        (i32.load)
        (set_global $fp)
        (set_global $sp)
        ;; (ExprStmt ("f(x)" "[void]" (Var "print" "0,1" "[(*char)void]") ( (String "[*char]" "0"))))
        (i32.const 0)
        (get_global $sp)
        (get_global $fp)
        (i32.store)
        (get_global $sp)
        (set_global $fp)
        (call $print)
        (get_global $fp)
        (get_global $fp)
        (i32.load)
        (set_global $fp)
        (set_global $sp)
        ;; (ForStmt (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 0))) (< "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 4)) (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (+ "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 1)))) (Block 7 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (v[x] "[integer]" (Var "j" "6,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "5,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (Integer "[integer]" 8))))))
        ;; (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 0)))
        (get_global $fp)
        (i32.const 72)
        (i32.add)
        (i32.const 0)
        (set_global $temp_i32)
        (get_global $temp_i32)
        (i32.store)
        (get_global $temp_i32)
        (drop)
        (block $loopbrk1
        (loop $loopaux1
        (block $loop1
        (get_global $fp)
        (i32.const 72)
        (i32.add)
        (i32.load)
        (i32.const 4)
        (i32.lt_s)
        (i32.eqz)
        (br_if $loopbrk1)
        ;; (Block 7 (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (v[x] "[integer]" (Var "j" "6,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "5,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (Integer "[integer]" 8)))))
        ;; {
          ;; (ExprStmt ("f(x)" "[void]" (Var "print_int_pad" "0,7" "[(integer, integer)void]") ( (v[x] "[integer]" (Var "j" "6,0" "[integer]") (v[x] "[[4]integer]" (Var "i" "5,0" "[integer]") (Var "mat" "1,0" "[[4][4]integer]"))) (Integer "[integer]" 8))))
          (get_global $fp)
          (i32.const 4)
          (i32.add)
          (i32.const 16)
          (get_global $fp)
          (i32.const 68)
          (i32.add)
          (i32.load)
          (i32.mul)
          (i32.add)
          (i32.const 4)
          (get_global $fp)
          (i32.const 72)
          (i32.add)
          (i32.load)
          (i32.mul)
          (i32.add)
          (i32.load)
          (i32.const 8)
          (get_global $sp)
          (get_global $fp)
          (i32.store)
          (get_global $sp)
          (set_global $fp)
          (call $print_int_pad)
          (get_global $fp)
          (get_global $fp)
          (i32.load)
          (set_global $fp)
          (set_global $sp)
        ;; }
        )
        ;; (ExprStmt (= "[integer]" (Var "j" "6,0" "[integer]") (+ "[integer]" (Var "j" "6,0" "[integer]") (Integer "[integer]" 1))))
        (get_global $fp)
        (i32.const 72)
        (i32.add)
        (get_global $fp)
        (i32.const 72)
        (i32.add)
        (i32.load)
        (i32.const 1)
        (i32.add)
        (set_global $temp_i32)
        (get_global $temp_i32)
        (i32.store)
        (get_global $temp_i32)
        (drop)
        (br $loopaux1)
        )
        )
        ;; (ExprStmt ("f(x)" "[void]" (Var "_ln" "0,0" "[()void]") ()))
        (get_global $sp)
        (get_global $fp)
        (i32.store)
        (get_global $sp)
        (set_global $fp)
        (call $_ln)
        (get_global $fp)
        (get_global $fp)
        (i32.load)
        (set_global $fp)
        (set_global $sp)
      ;; }
      )
      ;; (ExprStmt (= "[integer]" (Var "i" "5,0" "[integer]") (+ "[integer]" (Var "i" "5,0" "[integer]") (Integer "[integer]" 1))))
      (get_global $fp)
      (i32.const 68)
      (i32.add)
      (get_global $fp)
      (i32.const 68)
      (i32.add)
      (i32.load)
      (i32.const 1)
      (i32.add)
      (set_global $temp_i32)
      (get_global $temp_i32)
      (i32.store)
      (get_global $temp_i32)
      (drop)
      (br $loopaux0)
      )
      )
    ;; }
    ;; (return (Integer "[integer]" 0))
    (i32.const 0)
    (return)
  ;; }
  (get_global $sp)
  (i32.const 84)
  (i32.sub)
  (set_global $sp)
  )

)
