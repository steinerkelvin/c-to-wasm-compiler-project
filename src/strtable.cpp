#include <cassert>
#include <map>
#include <string>
#include <vector>

#include "strtable.hpp"

static std::map<std::string, size_t> str_table;
static std::vector<std::string> str_list;

size_t strtb_add(char *cstr) {
    const std::string str(cstr);
    auto it = str_table.find(str);
    if (it != str_table.end()) {
        return it->second;
    }
    str_list.push_back(str);
    const size_t pos = str_list.size() - 1;
    str_table[str] = pos;
    return pos;
}

const char *strtb_get(size_t i) {
    assert(i >= 0);
    assert(i < str_list.size());
    return str_list[i].c_str();
}

void strtb_print(FILE *f) {
    size_t i = 0;
    for (auto &str : str_list) {
        fprintf(f, "String %2lu -- %s\n", i, str.c_str());
        i++;
    }
}
