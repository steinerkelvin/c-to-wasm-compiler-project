#if !defined(SYMTABLE_H)
#define SYMTABLE_H
#include <stdbool.h>

void open_scope();
void close_scope();

void insert_typename(const char *name);
bool is_typename(const char *name);

#endif // SYMTABLE_H
