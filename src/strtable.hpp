#if !defined(STRTABLE_H)
#define STRTABLE_H

/**
 * @file strtable.hpp
 */

#include <ostream>
#include <string>

using StrId = size_t;
struct StrRef {
    StrId id;
    const std::string& get() const;
};

namespace strtb {

/// Adds string to string table.
StrRef add(const std::string& str);

/// Writes a representation of the entire string table into `stream`.
std::ostream& repr(std::ostream& stream);

}; // namespace strtb

#endif /* STRTABLE_H */
