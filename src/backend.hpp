#if !defined(BACKEND_H)
#define BACKEND_H

#include <cstddef>
#include <ostream>

#include "ast.hpp"

namespace back {
extern const size_t null_size;
extern const size_t word_size;
extern const size_t base_activ_record_size;

/**
 * Consumes an AST generating code
 */
void generate_code(
    std::ostream& out, ast::Program* root, size_t strtb_size, size_t data_size);

}; // namespace back

#endif /* BACKEND_H */
