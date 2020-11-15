#include "symtable.hpp"
#include <cassert>
#include <map>
#include <string>
#include <vector>

using sbtb::NameRef;
using sbtb::NameRow;
using sbtb::TagRef;
using sbtb::TagRow;
using sbtb::TypeRef;
using sbtb::TypeRow;
using types::Type;

struct Scope {
    using IdMap = std::map<std::string, SymId>;

    IdMap tags_map;
    IdMap types_map;
    IdMap names_map;

    std::vector<TagRow> tags;
    std::vector<TypeRow> types;
    std::vector<NameRow> names;

    Scope(const ScopeId id, const std::optional<ScopeId> parent)
        : id(id), parent(parent){};

    SymId add_tag(const std::string& name, Type* type)
    {
        assert(tags_map.find(name) == tags_map.end());
        const SymId idx = tags.size();
        TagRow row = {{name, type}};
        tags.push_back(row);
        tags_map[name] = idx;
        return idx;
    }
    SymId add_type(const std::string& name, Type* type)
    {
        assert(types_map.find(name) == types_map.end());
        const SymId idx = types.size();
        TypeRow row = {{name, type}};
        types.push_back(row);
        types_map[name] = idx;
        return idx;
    }
    SymId add_name(const std::string& name, Type* type)
    {
        assert(names_map.find(name) == names_map.end());
        const SymId idx = names.size();
        NameRow row = {{name, type}};
        names.push_back(row);
        names_map[name] = idx;
        return idx;
    }

  protected:
    const ScopeId id;
    const std::optional<ScopeId> parent;
};

static std::vector<Scope> scopes;
static std::vector<ScopeId> scope_stack;

namespace sbtb {

ScopeId init()
{
    assert(scope_stack.size() == 0);
    const size_t idx = scopes.size();
    const Scope new_scope = Scope(idx, std::optional<ScopeId>());
    scopes.push_back(new_scope);
    scope_stack.push_back(idx);
    return idx;
}

ScopeId open_scope()
{
    assert(scope_stack.size() > 0);
    const size_t parent = *(scope_stack.end() - 1);
    const size_t idx = scopes.size();
    const Scope new_scope = Scope(idx, std::optional<ScopeId>(parent));
    scopes.push_back(new_scope);
    scope_stack.push_back(idx);
    return idx;
}
void close_scope()
{
    assert(scope_stack.size() > 0);
    scope_stack.pop_back();
}

TagRow& TagRef::get()
{
    assert(scopes.size() > this->scope_id);
    Scope& scope = scopes[this->scope_id];
    assert(scope.tags.size() > this->sym_id);
    return scope.tags[this->sym_id];
}
TypeRow& TypeRef::get()
{
    assert(scopes.size() > this->scope_id);
    Scope& scope = scopes[this->scope_id];
    assert(scope.types.size() > this->sym_id);
    return scope.types[this->sym_id];
}
NameRow& NameRef::get()
{
    assert(scopes.size() > this->scope_id);
    Scope& scope = scopes[this->scope_id];
    assert(scope.names.size() > this->sym_id);
    return scope.names[this->sym_id];
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

NameRef insert_name(const std::string& name, Type* type)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const SymId sym_id = scope.add_name(name, type);
    return NameRef{{scope_id, sym_id}};
}

std::optional<TagRef>
lookup_tag(const std::string& name, bool last_scope)
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

std::optional<TypeRef>
lookup_type(const std::string& name, bool last_scope)
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

std::optional<NameRef>
lookup_name(const std::string& name, bool last_scope)
{
    assert(scope_stack.size() > 0);
    if (!last_scope)
        for (const auto scope_id : scope_stack) {
            Scope& scope = scopes[scope_id];
            const auto& map = scope.names_map;
            const auto& it = map.find(name);
            if (it != map.end()) {
                const SymId sym_id = it->second;
                return {(NameRef){{scope_id, sym_id}}};
            }
        }
    else {
        const ScopeId scope_id = *(scope_stack.end() - 1);
        Scope& scope = scopes[scope_id];
        const auto& map = scope.names_map;
        const auto& it = map.find(name);
        if (it != map.end()) {
            const SymId sym_id = it->second;
            return {(NameRef){{scope_id, sym_id}}};
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

} // namespace sbtb
