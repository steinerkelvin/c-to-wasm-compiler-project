#if !defined(UTIL_H)
#define UTIL_H

#define ALLOC(VAR) ((typeof(VAR)) malloc(sizeof(typeof(*(VAR)))))

#endif // UTIL_H
