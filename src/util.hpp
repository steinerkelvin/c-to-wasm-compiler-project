#if !defined(UTIL_H)
#define UTIL_H

/**
 * @file util.hpp
 */

#include <cstddef>
// #include <functional>

#define ALLOC(VAR) ((typeof(VAR))malloc(sizeof(typeof(*(VAR)))))

#define return_if(val) if (val) return;

#define assert_ret(val) (assert(val), val)
#define assert_derref(val) (assert(val), *(val))
#if NDEBUG
#define assert_ret(val) (val)
#define assert_derref(val) (*(val))
#endif

template <typename T>
T div_ceil(T a, T b)
{
    return a / b + (a % b != 0);
}

// Resolves to true only once
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
    size_t next() { return (n++); }
    void prev() { n--; }
    size_t reset() { return (n = 0); }
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
