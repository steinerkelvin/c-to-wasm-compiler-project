#if !defined(STRTABLE_H)
#define STRTABLE_H

/**
 * @file strtable.hpp
 */

#include <functional>
#include <optional>
#include <ostream>
#include <string>

struct StrRow {
    const std::string content;
    std::optional<size_t> offset;

    StrRow(const std::string& content) : content(content) {}
};

using StrId = size_t;
struct StrRef {
    StrId id;
    StrRow& get() const;
};

namespace strtb {

/// Adds string to string table.
StrRef add(const std::string& str);

/// Writes a representation of the entire string table into `stream`.
std::ostream& repr(std::ostream& stream);

size_t compute_offsets();

/// Iterate through all string table rows
void visit(std::function<void(const StrRow&)> visitor);

}; // namespace strtb

#endif /* STRTABLE_H */
