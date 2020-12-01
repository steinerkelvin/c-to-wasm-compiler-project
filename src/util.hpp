#if !defined(UTIL_H)
#define UTIL_H

/**
 * @file util.hpp
 */

#include <cstddef>

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

struct Counter {
    size_t next() { return (n++); };
    size_t reset() { return (n = 0); };
    operator size_t() { return n; }

  protected:
    size_t n = 0;
};

/*
template <typename T, typename B>
void action_if(B* value, std::function<void(T*)> action)
{
    auto cast_value = dynamic_cast<T*>(value);
    if (cast_value != nullptr) {
        action(cast_value);
    }
}
*/

#endif // UTIL_H
