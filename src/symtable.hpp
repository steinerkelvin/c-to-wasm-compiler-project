#if !defined(SYMTABLE_H)
#define SYMTABLE_H

/**
 * @file symtable.hpp
 */

#include <cstddef>
#include <map>
#include <optional>
#include <string>

#include "types.hpp"

using ScopeId = size_t;
using SymId = size_t;

namespace symtb {

struct Row {
    std::string name;
    types::Type* type;

  protected:
    Row(const std::string& name, types::Type* type) : name(name), type(type) {}
};

struct TagRow : Row {
    TagRow(const std::string& name, types::Type* type) : Row(name, type) {}
};
struct TypeRow : Row {
    TypeRow(const std::string& name, types::Type* type) : Row(name, type) {}
};
struct VarRow : Row {
    const bool is_param;
    const bool is_global;
    std::optional<size_t> offset;
    VarRow(
        const std::string& name,
        types::Type* type,
        bool is_param = false,
        bool is_global = false)
        : Row(name, type), is_param(is_param), is_global(is_global)
    {}
};

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
struct VarRef : SymRef {
    VarRow& get() const;
};

struct Scope {
    const ScopeId id;
    const std::optional<ScopeId> parent_id;
    const bool is_func_scope;
    const bool is_global_scope;

    std::optional<size_t> base_offset;
    std::optional<size_t> size;

    using IdMap = std::map<std::string, SymId>;
    IdMap tags_map;
    IdMap types_map;
    IdMap vars_map;

    std::vector<TagRow> tags;
    std::vector<TypeRow> types;
    std::vector<VarRow> vars;

    Scope(
        const ScopeId id,
        const std::optional<ScopeId> parent_id,
        bool is_func_scope = false,
        bool is_global_scope = false)
        : id(id), parent_id(parent_id), is_func_scope(is_func_scope),
          is_global_scope(is_global_scope){};

    const ScopeId get_id() const { return this->id; };
    const std::optional<ScopeId> get_parent() const { return this->parent_id; };

    SymId add_tag(const std::string& name, types::Type* type);
    SymId add_type(const std::string& name, types::Type* type);
    SymId
    add_var(const std::string& name, types::Type* type, bool is_param = false);

    std::optional<VarRef> lookup_var(const std::string& name);
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
ScopeId open_scope(bool is_func_scope = false);

/** Closes the last opened scope. */
void close_scope();

/**
 * Gets a scope given its id.
 *
 */
Scope& get_scope(ScopeId id);

/** Inserts enum/struct/union tag into the last opened scope. */
TagRef insert_tag(const std::string& namep, types::Type* type);
/** Inserts typedef'ed name into the last opened scope. */
TypeRef insert_typename(const std::string& namep, types::Type* type);
/** Inserts variable name into the last opened scope. */
VarRef
insert_var(const std::string& namep, types::Type* type, bool is_param = false);

TagRow& get(const TagRef& ref);
TypeRow& get(const TypeRef& ref);
VarRow& get(const VarRef& ref);

std::optional<TagRef>
lookup_tag(const std::string& name, bool last_scope = false);
std::optional<TypeRef>
lookup_type(const std::string& name, bool last_scope = false);
std::optional<VarRef>
lookup_var(const std::string& name, bool last_scope = false);

/* Verifica se um nome de tipo está em escopo (em qualquer nível) */
bool is_typename(const char* name);

/**
 * Computes scope sizes and offsets of all variables on all scopes
 */
void compute_offsets(size_t base_activ_record_size);

/**
 * Gets size of the global scope (first scope)
 */
size_t get_global_scope_size();

} // namespace symtb

#endif // SYMTABLE_H
