#include "symtable.h"
#include <cassert>
#include <map>
#include <string>
#include <vector>

using scope_table_t = std::map<std::string, bool>;

static std::vector<scope_table_t> table{scope_table_t()};

void open_scope() {
    table.push_back(scope_table_t());
};
void close_scope() {
    assert(table.size() > 0);
    table.pop_back();
};

void insert_typename(const char *namep) {
    assert(table.size() > 0);
    const int last = table.size() - 1;
    auto &scope = table[last];
    std::string name(namep);
    scope[name] = true;
};

bool is_typename(const char *namep) {
    const std::string name(namep);
    for (auto &scope : table) {
        auto it = scope.find(name);
        if (it != scope.end()) {
            return true;
        }
    }
    return false;
}
