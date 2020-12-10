#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "strtable.hpp"

namespace strtb {

static std::unordered_map<std::string, StrId> str_table;
static std::vector<StrRow> str_list;

StrRef add(const std::string& str_raw)
{
    const auto len = str_raw.length();
    assert(str_raw[0] == '\"');
    assert(str_raw[len - 1] == '\"');

    // strip quotation marks
    auto str = str_raw.substr(1, len - 2);

    auto it = str_table.find(str);
    if (it != str_table.end()) {
        return StrRef{it->second};
    }
    str_list.push_back(StrRow(str));
    const StrId pos = str_list.size() - 1;
    str_table[str] = pos;
    return StrRef{pos};
}

static StrRow& get(const StrId i)
{
    assert(i >= 0);
    assert(i < str_list.size());
    return str_list[i];
}

std::ostream& repr(std::ostream& stream)
{
    size_t i = 0;
    for (auto& str_row : str_list) {
        auto& str = str_row.content;
        stream << "String " << i << " -- " << str << std::endl;
        i++;
    }
    return stream;
}

size_t compute_offsets()
{
    size_t size = 0;
    for (auto& str_row : str_list) {
        str_row.offset = size;
        size += str_row.content.size() + 1;
    }
    return size;
}

void visit(std::function<void(const StrRow&)> visitor)
{
    for (const auto& str_row : str_list) {
        visitor(str_row);
    }
}

} // namespace strtb

StrRow& StrRef::get() const { return strtb::get(this->id); }
