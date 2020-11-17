#include <cassert>
#include <unordered_map>
#include <string>
#include <vector>

#include "strtable.hpp"

namespace strtb {

static std::unordered_map<std::string, StrId> str_table;
static std::vector<std::string> str_list;

StrRef add(const std::string& cstr)
{
    const std::string str(cstr);
    auto it = str_table.find(str);
    if (it != str_table.end()) {
        return StrRef{it->second};
    }
    str_list.push_back(str);
    const StrId pos = str_list.size() - 1;
    str_table[str] = pos;
    return StrRef{pos};
}

const std::string& get(const StrId i)
{
    assert(i >= 0);
    assert(i < str_list.size());
    return str_list[i];
}

std::ostream& repr(std::ostream& stream)
{
    size_t i = 0;
    for (auto& str : str_list) {
        stream << "String " << i << " -- " << str << std::endl;
        i++;
    }
    return stream;
}

} // namespace strtb

const std::string& StrRef::get() const {
    return strtb::get(this->id);
}
