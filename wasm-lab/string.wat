(module
    (import "runtime" "println" (func $println (param i32)))
    (import "runtime" "println_int" (func $println_int (param i32)))
    (import "runtime" "println_real" (func $println_real (param f32)))

    (func $main
        (i32.store8 (i32.const 0) (i32.const 72))
        (i32.store8 (i32.const 1) (i32.const 101))
        (i32.store8 (i32.const 2) (i32.const 108))
        (i32.store8 (i32.const 3) (i32.const 108))
        (i32.store8 (i32.const 4) (i32.const 111))
        (i32.store8 (i32.const 5) (i32.const 0))
        i32.const 0
        call $println
    )

    (memory $mem 1)
    (export "memory" (memory $mem))
    (export "main" (func $main))
)
