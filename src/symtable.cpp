#include "symtable.hpp"
#include <cassert>
#include <iostream>
#include <vector>

static std::vector<symtb::Scope> scopes;
static std::vector<ScopeId> scope_stack;

namespace symtb {
using symtb::TagRef;
using symtb::TagRow;
using symtb::TypeRef;
using symtb::TypeRow;
using symtb::VarRef;
using symtb::VarRow;
using types::Type;

SymId Scope::add_tag(const std::string& name, Type* type)
{
    assert(tags_map.find(name) == tags_map.end());
    const SymId idx = tags.size();
    TagRow row(name, type);
    tags.push_back(row);
    tags_map[name] = idx;
    return idx;
}
SymId Scope::add_type(const std::string& name, Type* type)
{
    assert(types_map.find(name) == types_map.end());
    const SymId idx = types.size();
    TypeRow row(name, type);
    types.push_back(row);
    types_map[name] = idx;
    return idx;
}
SymId Scope::add_var(const std::string& name, Type* type, bool is_param)
{
    assert(vars_map.find(name) == vars_map.end());
    VarRow row(name, type, is_param, this->is_global_scope);
    const SymId idx = vars.size();
    vars.push_back(row);
    vars_map[name] = idx;
    return idx;
}

std::optional<VarRef> Scope::lookup_var(const std::string& name)
{
    auto& map = this->vars_map;
    const auto& it = map.find(name);
    if (it != map.end()) {
        const SymId sym_id = it->second;
        return {(VarRef){{this->id, sym_id}}};
    }
    return std::nullopt;
}

ScopeId init()
{
    assert(scope_stack.size() == 0);
    const size_t idx = scopes.size();
    const Scope new_scope = Scope(idx, std::optional<ScopeId>(), false, true);
    scopes.push_back(new_scope);
    scope_stack.push_back(idx);
    return idx;
}

ScopeId open_scope(bool is_func_scope)
{
    assert(scope_stack.size() > 0);
    const size_t parent = *(scope_stack.end() - 1);
    const size_t idx = scopes.size();
    const Scope new_scope =
        Scope(idx, std::optional<ScopeId>(parent), is_func_scope);
    scopes.push_back(new_scope);
    scope_stack.push_back(idx);
    return idx;
}
void close_scope()
{
    assert(scope_stack.size() > 0);
    scope_stack.pop_back();
}

Scope& get_scope(ScopeId id)
{
    assert(id < scopes.size());
    return scopes[id];
}

TagRow& TagRef::get() const
{
    assert(scopes.size() > this->scope_id);
    Scope& scope = scopes[this->scope_id];
    assert(scope.tags.size() > this->sym_id);
    return scope.tags[this->sym_id];
}
TypeRow& TypeRef::get() const
{
    assert(scopes.size() > this->scope_id);
    Scope& scope = scopes[this->scope_id];
    assert(scope.types.size() > this->sym_id);
    return scope.types[this->sym_id];
}
VarRow& VarRef::get() const
{
    assert(scopes.size() > this->scope_id);
    Scope& scope = scopes[this->scope_id];
    assert(scope.vars.size() > this->sym_id);
    return scope.vars[this->sym_id];
}

TagRef insert_tag(const std::string& name, Type* type)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const SymId sym_id = scope.add_tag(name, type);
    return TagRef{{scope_id, sym_id}};
}

TypeRef insert_typename(const std::string& name, Type* type)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const SymId sym_id = scope.add_type(name, type);
    return TypeRef{{scope_id, sym_id}};
}

VarRef insert_var(const std::string& name, Type* type, bool is_param)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const SymId sym_id = scope.add_var(name, type, is_param);
    return VarRef{{scope_id, sym_id}};
}

std::optional<TagRef> lookup_tag(const std::string& name, bool last_scope)
{
    assert(scope_stack.size() > 0);
    if (!last_scope)
        for (const auto scope_id : scope_stack) {
            Scope& scope = scopes[scope_id];
            const auto& map = scope.tags_map;
            const auto& it = map.find(name);
            if (it != map.end()) {
                const SymId sym_id = it->second;
                return {(TagRef){{scope_id, sym_id}}};
            }
        }
    else {
        const ScopeId scope_id = *(scope_stack.end() - 1);
        Scope& scope = scopes[scope_id];
        const auto& map = scope.tags_map;
        const auto& it = map.find(name);
        if (it != map.end()) {
            const SymId sym_id = it->second;
            return {(TagRef){{scope_id, sym_id}}};
        }
    }
    return {};
}

std::optional<TypeRef> lookup_type(const std::string& name, bool last_scope)
{
    assert(scope_stack.size() > 0);
    if (!last_scope)
        for (const auto scope_id : scope_stack) {
            Scope& scope = scopes[scope_id];
            const auto& map = scope.types_map;
            const auto& it = map.find(name);
            if (it != map.end()) {
                const SymId sym_id = it->second;
                return {(TypeRef){{scope_id, sym_id}}};
            }
        }
    else {
        const ScopeId scope_id = *(scope_stack.end() - 1);
        Scope& scope = scopes[scope_id];
        const auto& map = scope.types_map;
        const auto& it = map.find(name);
        if (it != map.end()) {
            const SymId sym_id = it->second;
            return {(TypeRef){{scope_id, sym_id}}};
        }
    }
    return {};
}

std::optional<VarRef> lookup_var(const std::string& name, bool last_scope)
{
    assert(scope_stack.size() > 0);
    if (!last_scope)
        for (const auto scope_id : scope_stack) {
            Scope& scope = scopes[scope_id];
            const auto& map = scope.vars_map;
            const auto& it = map.find(name);
            if (it != map.end()) {
                const SymId sym_id = it->second;
                return {(VarRef){{scope_id, sym_id}}};
            }
        }
    else {
        const ScopeId scope_id = *(scope_stack.end() - 1);
        Scope& scope = scopes[scope_id];
        const auto& map = scope.vars_map;
        const auto& it = map.find(name);
        if (it != map.end()) {
            const SymId sym_id = it->second;
            return {(VarRef){{scope_id, sym_id}}};
        }
    }
    return {};
}

bool is_typename(const char* namep)
{
    const std::string name(namep);
    for (auto& scope_id : scope_stack) {
        const auto& scope = scopes[scope_id];
        const auto& map = scope.types_map;
        const auto it = map.find(name);
        if (it != map.end()) {
            return true;
        }
    }
    return false;
}

void compute_offsets(size_t base_activ_record_size)
{
    for (auto& scope : scopes) {
        if (!scope.parent_id || scope.is_func_scope) {
            scope.base_offset = 0;
        } else {
            size_t par_id = *(scope.parent_id);
            auto& parent = scopes[par_id];
            auto parent_base = assert_derref(parent.base_offset);
            auto parent_size = assert_derref(parent.size);
            scope.base_offset = parent_base + parent_size;
        }

        const size_t base_offset = *scope.base_offset;
        scope.size = 0;
        auto& scope_size = *scope.size;

        // TODO compute space and offset for arguments before?
        if (scope.is_func_scope) {
            scope_size += base_activ_record_size;
        }

        for (auto& var_row : scope.vars) {
            var_row.offset = base_offset + scope_size;
            const size_t var_size = var_row.type->get_size();
            scope_size += var_size;
            // std::cerr << var_row.name << " : " << *(var_row.type) << " : "
            //           << var_size << " : " << *(var_row.offset) << std::endl;
        }
    }
}

size_t get_global_scope_size()
{
    assert(scopes.size() > 0);
    auto size_opt = scopes[0].size;
    assert(size_opt);
    return *size_opt;
}

} // namespace symtb
