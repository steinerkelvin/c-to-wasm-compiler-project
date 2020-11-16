#if !defined(UTIL_H)
#define UTIL_H

#define ALLOC(VAR) ((typeof(VAR))malloc(sizeof(typeof(*(VAR)))))

// Estrutura que resolve para verdadeiro apenas a primeira vez
struct Once {
    operator bool()
    {
        const bool result = flag;
        flag = false;
        return result;
    }
    void reset() {
        flag = true;
    }
  protected:
    bool flag = true;
};

#endif // UTIL_H
