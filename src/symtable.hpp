#if !defined(SYMTABLE_H)
#define SYMTABLE_H
#include <cstdlib>

using ScopeRef = size_t;

/* Cria um novo espopo para nomes. Os nomes serão acidionados no último escopo
 * aberto.
 */
ScopeRef open_scope();
/* Fecha o último escopo aberto */
void close_scope();

/* Insere um nome de tipo no último escopo */
void insert_typename(const char *name);
/* Verifica se um nome de tipo está em escopo (em qualquer nível) */
bool is_typename(const char *name);

#endif // SYMTABLE_H
