#if !defined(UTIL_H)
#define UTIL_H

/**
 * @file util.hpp
 */

#define ALLOC(VAR) ((typeof(VAR))malloc(sizeof(typeof(*(VAR)))))

#define assert_ret(val) (assert(val), val)
#define assert_derref(val) (assert(val), *(val))
#if NDEBUG
#define assert_ret(val) (val)
#define assert_derref(val) (*(val))
#endif

// Estrutura que resolve para verdadeiro apenas a primeira vez
struct Once {
    operator bool()
    {
        const bool result = flag;
        flag = false;
        return result;
    }
    void reset() { flag = true; }

  protected:
    bool flag = true;
};

#endif // UTIL_H
