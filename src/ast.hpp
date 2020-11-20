#if !defined(AST_H)
#define AST_H

/**
 * @file ast.hpp
 */

#include <algorithm>
#include <cassert>
#include <ostream>
#include <string>
#include <variant>
#include <vector>

#include "positions.hpp"
#include "strtable.hpp"
#include "symtable.hpp"
#include "types.hpp"

#define LABEL(TEXT)                                                            \
    virtual const char* const get_label() const { return (TEXT); };

namespace ast {

struct Node : pos::HasPosition {
    LABEL("NODE");

    virtual bool is_typed() const { return false; }
    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>();
    }
    virtual void write_data_repr(std::ostream& stream) const {}
    virtual void write_repr(std::ostream& stream) const
    {
        stream << "(" << this->get_label();
        this->write_data_repr(stream);
        for (auto const& child : this->get_children_nodes()) {
            stream << " ";
            child->write_repr(stream);
        }
        if (auto this_pos = this->get_pos()) {
            stream << " \"" << *this_pos << "\"";
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
struct SingleChildBase : R {
    SingleChildBase(T* child) : child(child) { assert(child); }
    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>{this->child};
    }

  protected:
    T* child;
};

// Root node of type R with two children of types T1 and T2
template <typename T1, typename T2, typename R>
struct TwoChildrenBase : R {
    TwoChildrenBase(T1* left, T2* right) : left(left), right(right)
    {
        assert(left);
        assert(right);
    }
    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>{this->left, this->right};
    }

  protected:
    T1* left;
    T2* right;
};

// Root node of type R with multiple children of type T
template <typename T, typename R = T>
struct MultiChildrenBase : R {
    virtual void add(T* child)
    {
        assert(child);
        this->children.push_back(child);
        this->merge_pos_from(child);
    };

    std::vector<T*>& get_children() { return this->children; }

    virtual const std::vector<Node*> get_children_nodes() const
    {
        // TODO descobrir se dá para usar um simples cast aqui?
        std::vector<Node*> result;
        std::copy(
            this->children.cbegin(), this->children.cend(),
            std::back_inserter(result));
        return result;
    }

    //   protected:
    std::vector<T*> children;
};

struct TypedNode : Node {
    virtual bool is_typed() const { return true; }
    types::Type* get_type() const
    {
        assert(this->type);
        return this->type;
    };

    void set_type(types::Type* t) { this->type = t; };
    void set_type(types::PrimType::PrimKind k)
    {
        this->type = new types::PrimType(k);
    };

    virtual void write_data_repr(std::ostream& stream) const
    {
        stream << " \"[" << *(this->type) << "]\"";
    }

  protected:
    types::Type* type = new types::PrimType{types::PrimType::VOID};
};

struct Expr : TypedNode {
    static Expr* retsame(Expr* child) { return child; };
};

struct Exprs : MultiChildrenBase<Expr, Node> {
    LABEL("");
};

struct Value : Expr {};

template <typename T, types::PrimType::PrimKind type_kind>
struct BaseValue : Expr {
    BaseValue(T value)
    {
        this->value = value;
        this->type = new types::PrimType{type_kind};
    };
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

struct IntegerValue : BaseValue<uint64_t, types::PrimType::INTEGER> {
    LABEL("Integer");
    IntegerValue(uint64_t value)
        : BaseValue<uint64_t, types::PrimType::INTEGER>(value)
    {}
};
struct FloatingValue : BaseValue<double, types::PrimType::REAL> {
    LABEL("Real");
    FloatingValue(double value)
        : BaseValue<double, types::PrimType::REAL>(value)
    {}
};
struct CharValue : BaseValue<char, types::PrimType::CHAR> {
    LABEL("Char");
    CharValue(char value) : BaseValue<char, types::PrimType::CHAR>(value) {}
    virtual void write_data_repr(std::ostream& stream) const
    {
        this->Expr::write_data_repr(stream);
        stream << " ";
        stream << (int)this->value;
    };
};

struct StringValue : Expr {
    LABEL("String");
    StringValue(StrRef ref) : ref(ref)
    {
        this->set_type(
            new types::Pointer(new types::PrimType(types::PrimType::CHAR), 1));
    };
    virtual void write_data_repr(std::ostream& stream) const
    {
        this->Expr::write_data_repr(stream);
        stream << " \"" << this->ref.id << "\"";
    };
    StrRef get_ref() const { return this->ref; }
    const std::string& get_str() const { return this->ref.get(); }

  protected:
    StrRef ref;
};

struct Variable : Expr {
    LABEL("Var");
    Variable(symtb::NameRef& ref) : ref(ref){};
    const std::string& get_name() const { return this->ref.get().name; };
    void write_data_repr(std::ostream& stream) const
    {
        stream << " \"" << this->get_name() << "\"";
        this->Expr::write_data_repr(stream);
    };

  protected:
    const symtb::NameRef ref;
};

// Type coersion node
struct Coersion : SingleChildBase<Expr> {
    using SingleChildBase<Expr>::SingleChildBase;
};
using CoersionBuilder = std::function<Expr*(Expr*)>;

// integer to real
struct I2R : Coersion {
    LABEL("I2R");
    I2R(Expr* base) : Coersion(base)
    {
        this->type = new types::PrimType(types::PrimType::REAL);
    }
};
extern const CoersionBuilder bdI2R;
// real to integer
struct R2I : Coersion {
    LABEL("R2I");
    R2I(Expr* base) : Coersion(base)
    {
        this->type = new types::PrimType(types::PrimType::INTEGER);
    }
};
extern const CoersionBuilder bdR2I;
// integer to char
struct I2C : Coersion {
    LABEL("I2C");
    I2C(Expr* base) : Coersion(base)
    {
        this->type = new types::PrimType(types::PrimType::CHAR);
    }
};
extern const CoersionBuilder bdI2C;
// char to integer
struct C2I : Coersion {
    LABEL("C2I");
    C2I(Expr* base) : Coersion(base)
    {
        this->type = new types::PrimType(types::PrimType::INTEGER);
    }
};
extern const CoersionBuilder bdC2I;

extern const CoersionBuilder bdC2R;
extern const CoersionBuilder bdR2C;

// function to pointer
struct F2P : Coersion {
    LABEL("F2P");
    using Coersion::Coersion;
};
// vector to pointer
struct V2P : Coersion {
    LABEL("V2P");
    using Coersion::Coersion;
};

// Unary operator
template <typename T>
struct UnOp : SingleChildBase<Expr> {
    UnOp(Expr* child) : SingleChildBase<Expr>(child)
    {
        this->type = child->get_type();
        this->merge_pos_from(child);
    }
    static Expr* builder(Expr* child) { return new T(child); };
};

using UnBuilder = Expr* (*)(Expr* child);

// Binary operator
template <typename T>
struct BinOp : MultiChildrenBase<Expr> {
    // TODO ensure T = derived class (CRTP)
    BinOp(Expr* left, Expr* right)
    {
        assert(left != NULL);
        assert(right != NULL);
        this->children = std::vector<Expr*>{left, right};
        this->merge_pos_from(left);
        this->merge_pos_from(right);
    };
    static Expr* builder(Expr* left, Expr* right)
    {
        return new T(left, right);
    };
};

using BinBuilder = Expr* (*)(Expr* left, Expr* right);

struct Not : UnOp<Not> {
    LABEL("!");
    using UnOp::UnOp;
};
struct BitNot : UnOp<BitNot> {
    LABEL("~");
    using UnOp::UnOp;
};
struct InvertSignal : UnOp<InvertSignal> {
    LABEL("-");
    using UnOp::UnOp;
};
struct PosfixPlusPlus : UnOp<PosfixPlusPlus> {
    LABEL("++");
    using UnOp::UnOp;
};
struct PosfixMinusMinus : UnOp<PosfixMinusMinus> {
    LABEL("--");
    using UnOp::UnOp;
};
struct PrefixPlusPlus : UnOp<PrefixPlusPlus> {
    LABEL("p++");
    using UnOp::UnOp;
};
struct PrefixMinusMinus : UnOp<PrefixMinusMinus> {
    LABEL("p--");
    using UnOp::UnOp;
};

struct Plus : BinOp<Plus> {
    LABEL("+");
    using BinOp::BinOp;
};
struct Minus : BinOp<Minus> {
    LABEL("-");
    using BinOp::BinOp;
};
struct Times : BinOp<Times> {
    LABEL("*");
    using BinOp::BinOp;
};
struct Over : BinOp<Over> {
    LABEL("/");
    using BinOp::BinOp;
};
struct Mod : BinOp<Mod> {
    LABEL("%");
    using BinOp::BinOp;
};

struct Less : BinOp<Less> {
    LABEL("<");
    using BinOp::BinOp;
};
struct Greater : BinOp<Greater> {
    LABEL(">");
    using BinOp::BinOp;
};
struct LessEqual : BinOp<LessEqual> {
    LABEL("<=");
    using BinOp::BinOp;
};
struct GreaterEqual : BinOp<GreaterEqual> {
    LABEL(">=");
    using BinOp::BinOp;
};
struct Equal : BinOp<Equal> {
    LABEL("==");
    using BinOp::BinOp;
};
struct NotEqual : BinOp<NotEqual> {
    LABEL("==");
    using BinOp::BinOp;
};

struct And : BinOp<And> {
    LABEL("&&");
    using BinOp::BinOp;
};
struct Or : BinOp<Or> {
    LABEL("||");
    using BinOp::BinOp;
};

struct Assign : BinOp<Assign> {
    LABEL("=");
    using BinOp::BinOp;
};

struct AddressOf : UnOp<AddressOf> {
    LABEL("&x");
    using UnOp::UnOp;
};
struct Derreference : UnOp<Derreference> {
    LABEL("*x");
    using UnOp::UnOp;
};

struct IndexAccess : BinOp<IndexAccess> {
    LABEL("v[x]");
    using BinOp::BinOp;
};

struct Call : TwoChildrenBase<Expr, Exprs, Expr> {
    LABEL("\"f(x)\"");
    using TwoChildrenBase<Expr, Exprs, Expr>::TwoChildrenBase;
};

struct Statement : Node {
    LABEL("Statement");
};

struct Break : Statement {
    LABEL("break");
};
struct Continue : Statement {
    LABEL("continue");
};
struct Return : Statement {
    LABEL("return");
};
struct ReturnValue : SingleChildBase<Expr, Statement> {
    LABEL("return");
    using SingleChildBase::SingleChildBase;
};

struct IfStmt : Statement {
    LABEL("IfStmt");
    IfStmt(Expr* expr, Statement* stmt)
    {
        assert(expr);
        assert(stmt);
        this->expr = expr;
        this->stmt = stmt;
    }
    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>{this->expr, this->stmt};
    }

  protected:
    Expr* expr;
    Statement* stmt;
};

struct IfElseStmt : Statement {
    LABEL("IfElseStmt");
    IfElseStmt(Expr* expr, Statement* stmt, Statement* else_stmt)
    {
        assert(expr);
        assert(stmt);
        assert(else_stmt);
        this->expr = expr;
        this->stmt = stmt;
        this->else_stmt = else_stmt;
    }
    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>{this->expr, this->stmt, this->else_stmt};
    }

  protected:
    Expr* expr;
    Statement* else_stmt;
    Statement* stmt;
};

struct WhileStmt : Statement {
    LABEL("WhileStmt");
    WhileStmt(Expr* expr, Statement* stmt)
    {
        assert(expr);
        assert(stmt);
        this->expr = expr;
        this->stmt = stmt;
    }
    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>{this->expr, this->stmt};
    }

  protected:
    Expr* expr;
    Statement* stmt;
};

struct DoWhileStmt : Statement {
    LABEL("DoWhileStmt");
    DoWhileStmt(Expr* expr, Statement* stmt)
    {
        assert(expr);
        assert(stmt);
        this->expr = expr;
        this->stmt = stmt;
    }
    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>{this->expr, this->stmt};
    }

  protected:
    Expr* expr;
    Statement* stmt;
};

struct Block : MultiChildrenBase<Statement> {
    LABEL("Block");
    std::optional<ScopeId> scope_id;

    virtual void add(Statement* stmt)
    {
        // TODO
        // obs.: esse método inteiro pode ter removido em favor do método da
        // superclasse MultiChildrenBase que tem o assert
        // assert(stmt);
        if (stmt)
            this->MultiChildrenBase<Statement>::add(stmt);
    };
    void set_scope(const ScopeId scope_id) { this->scope_id = scope_id; }

    virtual void write_data_repr(std::ostream& stream) const
    {
        if (this->scope_id)
            stream << " " << this->scope_id.value();
    }
};

struct ExpressionStmt : SingleChildBase<Expr, Statement> {
    LABEL("ExpressionStmt");
    using SingleChildBase<Expr, Statement>::SingleChildBase;
};

struct Declaration : Node {
    LABEL("Declaration");
    string name;
};

struct FunctionDefinition : Declaration {
    LABEL("FunctionDefinition");
    FunctionDefinition(Block* body)
    {
        assert(body);
        this->body = body;
    }
    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>{this->body};
    }

  protected:
    Block* body;
};

struct Program : MultiChildrenBase<Declaration, Node> {
    LABEL("Program");
};

}; // namespace ast

#endif /* AST_H */
