#if !defined(AST_H)
#define AST_H

#include <cassert>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

enum class PrimitiveType {
    VOID,
    INT,
    CHAR,
    BOOL,
    FLOAT,
    DOUBLE,
};

typedef struct {
    PrimitiveType type;
} Type;

namespace Node {
struct BaseNode;
}
std::ostream& operator<<(std::ostream& stream, Node::BaseNode& node);

namespace Node {
using std::string;
using std::variant;
using std::vector;

struct BaseNode {
    virtual bool is_typed() { return false; };
    virtual void write_repr(std::ostream& stream){};
};

struct TypedNode : BaseNode {
    virtual bool is_typed() { return true; }
    Type get_type() { return this->type; };

  protected:
    Type type;
};

struct Expr : TypedNode {};

struct Value : Expr {};

template <typename T>
struct BaseValue : Expr {
    BaseValue(T value) { this->value = value; };
    virtual void write_repr(std::ostream& stream) { stream << this->value; };

  protected:
    T value;
};

struct IntegerValue : BaseValue<long> {
    IntegerValue(long value) : BaseValue<long>(value){};
};
struct FloatingValue : BaseValue<double> {
    FloatingValue(double value) : BaseValue<double>(value){};
};
struct CharValue : BaseValue<char> {
    CharValue(char value) : BaseValue<char>(value){};
};
struct StringValue : BaseValue<size_t> {
    StringValue(size_t value) : BaseValue<size_t>(value){};
};

struct Variable : Expr {
    size_t get_name() { return this->name; };
    Variable(size_t name) { this->name = name; };

  protected:
    size_t name;
};

struct UnOp : Expr {
    Expr* child;
    UnOp(Expr* child) {
        assert(child != NULL);
        this->type = child->get_type();
        this->child = child;
    }
    virtual void write_repr(std::ostream& stream) {
        BaseNode* nchield = static_cast<BaseNode*>(this->child);
        stream << "Value(" << (*nchield) << ")";
    }
};

struct BinOp : Expr {
    Expr* left;
    Expr* right;
    BinOp(Expr* left, Expr* right) {
        assert(left != NULL);
        assert(right != NULL);
        this->type = left->get_type(); // TODO
        this->left = left;
        this->right = right;
    }
    virtual void write_repr(std::ostream& stream) {
        stream << "BinOp(" << (*left) << ", " << (*right) << ")";
    }
};

struct Not : UnOp {
    using UnOp::UnOp;
};
struct BitNot : UnOp {
    using UnOp::UnOp;
};
struct InvertSignal : UnOp {
    using UnOp::UnOp;
};
struct PosfixPlusPlus : UnOp {
    using UnOp::UnOp;
};
struct PosfixMinusMinus : UnOp {
    using UnOp::UnOp;
};
struct PrefixPlusPlus : UnOp {
    using UnOp::UnOp;
};
struct PrefixMinusMinus : UnOp {
    using UnOp::UnOp;
};

struct Plus : BinOp {
    using BinOp::BinOp;
};
struct Minus : BinOp {
    using BinOp::BinOp;
};
struct Times : BinOp {
    using BinOp::BinOp;
};
struct Over : BinOp {
    using BinOp::BinOp;
};

struct Declaration : BaseNode {
    string name;
};
struct Statement : BaseNode {};

using DeclOrStmt = variant<Declaration, Statement>;
using CompoundStatement = std::vector<DeclOrStmt>;

struct FunctionDefinition : Declaration {
    CompoundStatement body;
};

using GlobalDeclaration = variant<Declaration, FunctionDefinition>;

struct Program : BaseNode {
    vector<GlobalDeclaration> children;
};

}; // namespace Node

#endif /* AST_H */
