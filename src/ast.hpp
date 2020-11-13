#if !defined(AST_H)
#define AST_H

#include <algorithm>
#include <cassert>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#include "symtable.hpp"
#include "types.hpp"

// TODO refatorar para template?
#define DECLARE_LABEL_STR(TEXT)                                                \
    virtual const char* const get_label() const { return (TEXT); };
#define DECLARE_LABEL(NAME) DECLARE_LABEL_STR(#NAME)

namespace ast {

struct Node {
    DECLARE_LABEL(Node);
    virtual bool is_typed() const { return false; }
    virtual const std::vector<Node*> get_children() const
    {
        return std::vector<Node*>();
    }
    virtual void write_data_repr(std::ostream& stream) const {}
    virtual void write_repr(std::ostream& stream) const
    {
        stream << "(" << this->get_label();
        this->write_data_repr(stream);
        for (auto const& child : this->get_children()) {
            stream << " ";
            child->write_repr(stream);
        }
        stream << ")";
    }
};
}; // namespace ast

std::ostream& operator<<(std::ostream& stream, const ast::Node& node);

namespace ast {
using std::string;
using std::variant;

// Node templates

// Root node of type R with single child of type T
template <typename T, typename R = T>
struct SingleNodeBase : R {
    virtual const std::vector<Node*> get_children() const
    {
        return std::vector<Node*>{this->child};
    }

  protected:
    T* child;
};

// Root node of type R with multiple children of type T
template <typename T, typename R = T>
struct MultiNodeBase : R {
    virtual const std::vector<Node*> get_children() const
    {
        std::vector<Node*> result;
        std::copy(
            this->children.cbegin(),
            this->children.cend(),
            std::back_inserter(result));
        return result;
    }

  protected:
    std::vector<T*> children;
};

struct TypedNode : Node {
    virtual bool is_typed() const { return true; }
    types::Type* get_type() const { return this->type; };

    void set_type(types::Type* t) { this->type = t; };
    void set_type(types::PrimKind k) { this->type = new types::PrimType{k}; };

    virtual void write_data_repr(std::ostream& stream) const
    {
        stream << " \"[" << *(this->type) << "]\"";
    }

  protected:
    types::Type *type = new types::PrimType{types::PrimKind::VOID};
};

struct Expr : TypedNode {};

struct Value : Expr {};

template <typename T>
struct BaseValue : Expr {
    BaseValue(T value) { this->value = value; };
    virtual void write_data_repr(std::ostream& stream) const
    {
        this->Expr::write_data_repr(stream);
        stream << " ";
        stream << this->value;
    };
    T get_value() const { return this->value; }

  protected:
    T value;
};

struct IntegerValue : BaseValue<long long> {
    DECLARE_LABEL(IntegerValue);
    IntegerValue(long long value) : BaseValue<long long>(value)
    {
        type = new types::PrimType{types::PrimKind::INTEGER};
    };
};
struct FloatingValue : BaseValue<double> {
    DECLARE_LABEL(FloatingValue);
    FloatingValue(double value) : BaseValue<double>(value)
    {
        type = new types::PrimType{types::PrimKind::REAL};
    };
};
struct CharValue : BaseValue<char> {
    DECLARE_LABEL(CharValue);
    CharValue(char value) : BaseValue<char>(value)
    {
        type = new types::PrimType{types::PrimKind::CHAR};
    };
};
struct StringValue : BaseValue<size_t> {
    DECLARE_LABEL(StringValue);
    StringValue(size_t value) : BaseValue<size_t>(value)
    {
        type = new types::PrimType{types::PrimKind::VOID}; // TODO
    };
};

struct Variable : Expr {
    DECLARE_LABEL(Variable);
    Variable(sbtb::NameRef& ref) { this->ref = ref; };
    const std::string& get_name() { return this->ref.get().name; };

  protected:
    sbtb::NameRef ref;
};

struct UnOp : SingleNodeBase<Expr> {
    UnOp(Expr* child)
    {
        assert(child != NULL);
        this->type = child->get_type();
        this->child = child;
    }
};

struct BinOp : MultiNodeBase<Expr> {
    BinOp(Expr* left, Expr* right)
    {
        assert(left != NULL);
        assert(right != NULL);
        this->children = std::vector<Expr*>{left, right};
        this->type = left->get_type(); // TODO
    }
};

struct Not : UnOp {
    DECLARE_LABEL_STR("!");
    using UnOp::UnOp;
};
struct BitNot : UnOp {
    DECLARE_LABEL_STR("~");
    using UnOp::UnOp;
};
struct InvertSignal : UnOp {
    DECLARE_LABEL_STR("-");
    using UnOp::UnOp;
};
struct PosfixPlusPlus : UnOp {
    DECLARE_LABEL_STR("++");
    using UnOp::UnOp;
};
struct PosfixMinusMinus : UnOp {
    DECLARE_LABEL_STR("--");
    using UnOp::UnOp;
};
struct PrefixPlusPlus : UnOp {
    DECLARE_LABEL_STR("p++");
    using UnOp::UnOp;
};
struct PrefixMinusMinus : UnOp {
    DECLARE_LABEL_STR("p--");
    using UnOp::UnOp;
};

struct Plus : BinOp {
    DECLARE_LABEL_STR("+");
    using BinOp::BinOp;
};
struct Minus : BinOp {
    DECLARE_LABEL_STR("-");
    using BinOp::BinOp;
};
struct Times : BinOp {
    DECLARE_LABEL_STR("*");
    using BinOp::BinOp;
};
struct Over : BinOp {
    DECLARE_LABEL_STR("/");
    using BinOp::BinOp;
};

struct IndexAccess : BinOp {
    DECLARE_LABEL_STR("[]");
    using BinOp::BinOp;
};

struct Statement : Node {
    DECLARE_LABEL(Statement);
};

struct IfStmt : Statement{
    DECLARE_LABEL(IfStmt);
    IfStmt(Expr* expr, Statement* stmt){
        this->expr = expr;
        this->stmt = stmt;
    }
    virtual const std::vector<Node*> get_children() const
    {
        return std::vector<Node*>{this->expr, this->stmt};
    }

protected:
    Expr* expr;
    Statement* stmt;
};

struct WhileStmt : Statement{
    DECLARE_LABEL(WhileStmt);
    WhileStmt(Expr* expr, Statement* stmt){
        this->expr = expr;
        this->stmt = stmt;
    }
    virtual const std::vector<Node*> get_children() const
    {
        return std::vector<Node*>{this->expr, this->stmt};
    }    

protected:
    Expr* expr;
    Statement* stmt;        
};

struct DoWhileStmt : Statement{
    DECLARE_LABEL(DoWhileStmt);
    DoWhileStmt(Expr* expr, Statement* stmt){
        this->expr = expr;
        this->stmt = stmt;
    }
    virtual const std::vector<Node*> get_children() const
    {
        return std::vector<Node*>{this->stmt, this->expr};
    }    

protected:
    Expr* expr;
    Statement* stmt;        
};

struct Block : MultiNodeBase<Statement> {
    DECLARE_LABEL(Block);
    std::optional<ScopeId> scope_id;

    void add(Statement* stmt)
    {
        // assert(stmt);  // TODO
        if (stmt)
            this->children.push_back(stmt);
    };
    void set_scope(const ScopeId scope_id) { this->scope_id = scope_id; }

    virtual void write_data_repr(std::ostream& stream) const
    {
        if (this->scope_id)
            stream << " " << this->scope_id.value();
    }
};

struct ExpressionStmt : SingleNodeBase<Expr, Statement> {
    DECLARE_LABEL(ExpressionStmt);
    ExpressionStmt(Expr* value)
    {
        assert(value);
        this->child = value;
    };
};

struct Declaration : Node {
    DECLARE_LABEL(Declaration);
    string name;
};

struct FunctionDefinition : Declaration {
    DECLARE_LABEL(FunctionDefinition);
    FunctionDefinition(Block* body)
    {
        assert(body);
        this->body = body;
    }
    virtual const std::vector<Node*> get_children() const
    {
        return std::vector<Node*>{this->body};
    }

  protected:
    Block* body;
};

struct Program : MultiNodeBase<Declaration, Node> {
    DECLARE_LABEL(Program);
    void add(Declaration* decl)
    {
        assert(decl);
        this->children.push_back(decl);
    };
};

}; // namespace ast

#endif /* AST_H */
