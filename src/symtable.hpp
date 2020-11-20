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

namespace symtb {

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

/**
 * Init symbol table.
 * Should be called once before the symbol table is used.
 */

ScopeId init();

/**
 * Creates a new scope for symbols to be declared into.
 * Symbols will be added in the last opened spoce.
 */
ScopeId open_scope();

/** Closes the last opened scope. */
void close_scope();

/** Inserts enum/struct/union tag into the last opened scope. */
TagRef insert_tag(const std::string& namep, types::Type* type);
/** Inserts typedef'ed name into the last opened scope. */
TypeRef insert_typename(const std::string& namep, types::Type* type);
/** Inserts variable name into the last opened scope. */
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

} // namespace symtb

#endif // SYMTABLE_H
