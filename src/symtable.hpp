#if !defined(SYMTABLE_H)
#define SYMTABLE_H

/**
 * @file symtable.hpp
 */

#include <cstddef>
#include <optional>
#include <string>

#include "types.hpp"

using ScopeId = size_t;
using SymId = size_t;

namespace sbtb {

struct Row {
    std::string name;
    types::Type* type;
};

struct TagRow : Row {};
struct TypeRow : Row {};
struct NameRow : Row {};

struct SymRef {
    ScopeId scope_id;
    SymId sym_id;
};
struct TagRef : SymRef {
    TagRow& get() const;
};
struct TypeRef : SymRef {
    TypeRow& get() const;
};
struct NameRef : SymRef {
    NameRow& get() const;
};

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

TagRef insert_tag(const std::string& namep, types::Type* type);
TypeRef insert_typename(const std::string& namep, types::Type* type);
NameRef insert_name(const std::string& namep, types::Type* type);

TagRow& get(const TagRef& ref);
TypeRow& get(const TypeRef& ref);
NameRow& get(const NameRef& ref);

std::optional<TagRef>
lookup_tag(const std::string& name, bool last_scope = false);
std::optional<TypeRef>
lookup_type(const std::string& name, bool last_scope = false);
std::optional<NameRef>
lookup_name(const std::string& name, bool last_scope = false);

/* Verifica se um nome de tipo está em escopo (em qualquer nível) */
bool is_typename(const char* name);

} // namespace sbtb

#endif // SYMTABLE_H
