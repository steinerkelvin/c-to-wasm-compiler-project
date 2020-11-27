(module
    (import "std" "println" (func $println (param i32)))
    (import "std" "println_int" (func $println_int (param i32)))
    (import "std" "println_real" (func $println_real (param f32)))

    (func $main
        (i32.const 42)
        (call $println_int)
    )

    (memory $mem 1)
    (export "memory" (memory $mem))
    (export "main" (func $main))
)
