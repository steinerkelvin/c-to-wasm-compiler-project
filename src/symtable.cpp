#include "symtable.hpp"
#include <cassert>
#include <map>
#include <string>
#include <vector>

using Scope = std::map<std::string, bool>;

static std::vector<Scope> scopes;
static std::vector<ScopeRef> scope_stack;

ScopeRef open_scope()
{
    size_t idx = scopes.size();
    scopes.push_back(Scope());
    scope_stack.push_back(idx);
    return idx;
};
void close_scope()
{
    assert(scope_stack.size() > 0);
    scope_stack.pop_back();
};

void insert_typename(const char* namep)
{
    assert(scope_stack.size() > 0);
    const ScopeRef scpref = *(scope_stack.end() - 1);
    Scope& scope = scopes[scpref];
    const std::string name(namep);
    scope[name] = true;
};

bool is_typename(const char* namep)
{
    const std::string name(namep);
    for (auto& scpref : scope_stack) {
        const auto& scope = scopes[scpref];
        const auto it = scope.find(name);
        if (it != scope.end()) {
            return true;
        }
    }
    return false;
}
