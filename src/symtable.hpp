#if !defined(SYMTABLE_H)
#define SYMTABLE_H
#include <cstddef>
#include <optional>
#include <string>

using ScopeId = size_t;
using SymId = size_t;
struct SymRef {
    ScopeId scope_id;
    SymId sym_id;
};

namespace sbtb {

/** Inicializa a tabela de símbolos */
ScopeId init();

/**
 * Cria um novo espopo para nomes. Os nomes serão acidionados no último escopo
 * aberto.
 */
ScopeId open_scope();

/* Fecha o último escopo aberto */
void close_scope();

/**
 * Inserem uma tag de enum/struct/union, nome de tipo ou nome de variável no
 * último escopo.
 */

SymRef insert_tag(const char* namep);
SymRef insert_type(const char* namep);
SymRef insert_name(const char* namep);

std::optional<SymRef> lookup_tag(const std::string& name);
std::optional<SymRef> lookup_type(const std::string& name);
std::optional<SymRef> lookup_name(const std::string& name);

/* Verifica se um nome de tipo está em escopo (em qualquer nível) */
bool is_typename(const char* name);

} // namespace sbtb

#endif // SYMTABLE_H
