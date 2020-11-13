#include "parsing.hpp"

namespace pars
{
    sbtb::NameRef get_var(const std::string& name) {
        auto pos = sbtb::lookup_name(name);
        if (pos) {
            return *pos;
        } else {
            fprintf(stderr, "SEMANTIC ERROR (%d): ", 0); // TODO
            fprintf(stderr, "variable '%s' not declared.\n", name.c_str());
            exit(1);
        }
    }
} // namespace pars
