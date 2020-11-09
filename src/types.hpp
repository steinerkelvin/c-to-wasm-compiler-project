#if !defined(TYPES_H)
#define TYPES_H

enum class PrimitiveType {
    VOID,
    INT,
    CHAR,
    BOOL,
    FLOAT,
    DOUBLE,
};

typedef struct {
    PrimitiveType type;
} Type;

#endif /* TYPES_H */
