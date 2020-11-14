#if !defined(UTIL_H)
#define UTIL_H

#define ALLOC(VAR) ((typeof(VAR))malloc(sizeof(typeof(*(VAR)))))

// Estrutura que resolve para verdadeiro apenas a primeira vez
struct Once {
    bool flag = true;
    operator bool() {
        const bool result = flag;
        flag = false;
        return result;
    }
};

#endif // UTIL_H
