#include "symtable.hpp"
#include <cassert>
#include <map>
#include <string>
#include <vector>

struct Scope {
    using IdMap = std::map<std::string, SymId>;

    IdMap tags_map;
    IdMap types_map;
    IdMap names_map;

    std::vector<void*> tags;
    std::vector<void*> types;
    std::vector<void*> names;

    Scope(const ScopeId id, const std::optional<ScopeId> parent)
        : id(id), parent(parent){};

    SymId add_tag(const std::string& name)
    {
        assert(tags_map.find(name) == tags_map.end());
        const SymId idx = tags.size();
        tags.push_back(NULL);
        tags_map[name] = idx;
        return idx;
    }
    SymId add_type(const std::string& name)
    {
        assert(types_map.find(name) == types_map.end());
        const SymId idx = types.size();
        types.push_back(NULL);
        types_map[name] = idx;
        return idx;
    }
    SymId add_name(const std::string& name)
    {
        assert(names_map.find(name) == names_map.end());
        const SymId idx = names.size();
        names.push_back(NULL);
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

SymRef insert_tag(const char* namep)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const SymId sym_id = scope.add_tag(std::string(namep));
    return SymRef{scope_id, sym_id};
}

SymRef insert_type(const char* namep)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const SymId sym_id = scope.add_type(std::string(namep));
    return SymRef{scope_id, sym_id};
}

SymRef insert_name(const char* namep)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const SymId sym_id = scope.add_name(std::string(namep));
    return SymRef{scope_id, sym_id};
}

std::optional<SymRef> lookup_tag(const std::string& name)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const auto& map = scope.tags_map;
    const auto& it = map.find(name);
    if (it != map.end()) {
        return {};
    }
    const SymId sym_id = it->second;
    return {(SymRef){scope_id, sym_id}};
}

std::optional<SymRef> lookup_type(const std::string& name)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const auto& map = scope.types_map;
    const auto& it = map.find(name);
    if (it == map.end()) {
        return {};
    }
    const SymId sym_id = it->second;
    return {(SymRef){scope_id, sym_id}};
}

std::optional<SymRef> lookup_name(const std::string& name)
{
    assert(scope_stack.size() > 0);
    const ScopeId scope_id = *(scope_stack.end() - 1);
    Scope& scope = scopes[scope_id];
    const auto& map = scope.names_map;
    const auto& it = map.find(name);
    if (it == map.end()) {
        return {};
    }
    const SymId sym_id = it->second;
    return {(SymRef){scope_id, sym_id}};
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
