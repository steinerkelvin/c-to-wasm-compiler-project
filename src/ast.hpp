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

class Node : public pos::HasPosition {
    LABEL("NODE");

  public:
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
        // if (auto this_pos = this->get_pos()) {
        //     stream << " \"" << *this_pos << "\"";
        // }
        stream << ")";
    }
};

}; // namespace ast

std::ostream& operator<<(std::ostream& stream, const ast::Node& node);

namespace ast {
using std::string;
using std::variant;

// Node templates

// Node with single child of type T
template <typename T>
class SingleChildBase : public virtual Node {
  protected:
    T* child;

  public:
    SingleChildBase(T* child) : child(child)
    {
        assert(child);
        this->merge_pos_from(child);
    }

    T*& get_child() { return this->child; }

    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>{this->child};
    }
};

// Node with two children of types T1 and T2
template <typename T1, typename T2>
class TwoChildrenBase : public virtual Node {
    T1* left;
    T2* right;

  public:
    TwoChildrenBase(T1* left, T2* right) : left(left), right(right)
    {
        assert(left);
        assert(right);
        this->merge_pos_from(left);
        this->merge_pos_from(right);
    }

    T1* get_left() { return left; }
    T2* get_right() { return right; }

    virtual const std::vector<Node*> get_children_nodes() const
    {
        return std::vector<Node*>{this->left, this->right};
    }
};

// Node with multiple children of type T
template <typename T>
class MultiChildrenBase : public virtual Node {
  protected:
    std::vector<T*> children;

  public:
    virtual void add(T* child)
    {
        assert(child);
        this->children.push_back(child);
        this->merge_pos_from(child);
    };

    const std::vector<T*>& get_children() { return this->children; }

    virtual const std::vector<Node*> get_children_nodes() const
    {
        // TODO descobrir se dá para usar um simples cast aqui?
        std::vector<Node*> result;
        std::copy(
            this->children.cbegin(), this->children.cend(),
            std::back_inserter(result));
        return result;
    }
};

//
// Expressions
//

struct TypedNode : virtual Node {
    // virtual bool is_typed() const { return true; }
    types::Type* get_type() const
    {
        assert(this->type);
        return this->type;
    };
    void set_type(types::Type* t) { this->type = t; };

  protected:
    types::Type* type = new types::PrimType{types::PrimType::VOID};
};

class Expr : public TypedNode {
  protected:
    Expr() {}

  public:
    virtual bool is_lvalue(bool flag) const { return false; };
    virtual bool is_rvalue(bool flag) const { return false; };

    virtual void write_data_repr(std::ostream& stream) const
    {
        stream << " \"[" << *(this->type) << "]\"";
    }

    static Expr* retsame(Expr* child) { return child; };
};

class LExpr : public virtual Expr {
    virtual bool is_lvalue(bool flag) const { return true; };
    // virtual bool is_rvalue(bool flag) const { return false; };
};

class RExpr : public virtual Expr {
  public:
    // virtual bool is_lvalue(bool flag) const { return false; };
    virtual bool is_rvalue(bool flag) const { return true; };
};

class LRExpr : public virtual LExpr, public virtual RExpr {
    // virtual bool is_lvalue(bool flag) const { return true; };
    // virtual bool is_rvalue(bool flag) const { return true; };
};

struct Exprs : MultiChildrenBase<Expr>, virtual Node {
    LABEL("");
};

template <typename T, types::PrimType::PrimKind type_kind>
struct BaseValue : RExpr {
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

struct StringValue : RExpr {
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

struct Variable : LRExpr {
    LABEL("Var");
    const symtb::VarRef ref;

    Variable(symtb::VarRef& ref) : ref(ref){};
    const std::string& get_name() const { return this->ref.get().name; };
    void write_data_repr(std::ostream& stream) const
    {
        stream << " \"" << this->get_name() << "\"";
        this->Expr::write_data_repr(stream);
    };
};

// Type coersion node
struct Coersion : SingleChildBase<Expr>, RExpr {
    using SingleChildBase::SingleChildBase;
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
struct UnOp : SingleChildBase<Expr>, RExpr {
    UnOp(Expr* child) : SingleChildBase(child)
    {
        this->type = child->get_type();
        this->merge_pos_from(child);
    }
    static Expr* builder(Expr* child) { return new T(child); };
};

// Unary lr-value operator
template <typename T>
struct LRUnOp : SingleChildBase<Expr>, LRExpr {
    LRUnOp(Expr* child) : SingleChildBase(child)
    {
        this->type = child->get_type();
        this->merge_pos_from(child);
    }
    static Expr* builder(Expr* child) { return new T(child); };
};

using UnBuilder = Expr* (*)(Expr* child);

// Binary operator

class TwoExpr : public TwoChildrenBase<Expr, Expr> {
    using TwoChildrenBase::TwoChildrenBase;
};

/// binary operator
struct BinOp : TwoExpr, virtual Expr {};
/// r-value binary operator
struct RBinOp : RExpr, virtual BinOp {};
/// lr-value binary operator
struct LRBinOp : LRExpr, virtual BinOp {};

template <typename T, typename R = RExpr>
class BinOpBase : public BinOp, public R {
  public:
    using TwoExpr::TwoExpr;
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

struct Plus : BinOpBase<Plus> {
    LABEL("+");
    using BinOpBase::BinOpBase;
};
struct Minus : BinOpBase<Minus> {
    LABEL("-");
    using BinOpBase::BinOpBase;
};
struct Times : BinOpBase<Times> {
    LABEL("*");
    using BinOpBase::BinOpBase;
};
struct Over : BinOpBase<Over> {
    LABEL("/");
    using BinOpBase::BinOpBase;
};
struct Mod : BinOpBase<Mod> {
    LABEL("%");
    using BinOpBase::BinOpBase;
};

struct Less : BinOpBase<Less> {
    LABEL("<");
    using BinOpBase::BinOpBase;
};
struct Greater : BinOpBase<Greater> {
    LABEL(">");
    using BinOpBase::BinOpBase;
};
struct LessEqual : BinOpBase<LessEqual> {
    LABEL("<=");
    using BinOpBase::BinOpBase;
};
struct GreaterEqual : BinOpBase<GreaterEqual> {
    LABEL(">=");
    using BinOpBase::BinOpBase;
};
struct Equal : BinOpBase<Equal> {
    LABEL("==");
    using BinOpBase::BinOpBase;
};
struct NotEqual : BinOpBase<NotEqual> {
    LABEL("==");
    using BinOpBase::BinOpBase;
};

struct And : BinOpBase<And> {
    LABEL("&&");
    using BinOpBase::BinOpBase;
};
struct Or : BinOpBase<Or> {
    LABEL("||");
    using BinOpBase::BinOpBase;
};

struct Assign : BinOpBase<Assign> {
    LABEL("=");
    using BinOpBase::BinOpBase;
};

struct AddressOf : UnOp<AddressOf> {
    LABEL("&x");
    using UnOp::UnOp;
};

struct Derreference : LRUnOp<Derreference> {
    LABEL("*x");
    using LRUnOp::LRUnOp;
};

struct IndexAccess : BinOpBase<IndexAccess, LRExpr> {
    LABEL("v[x]");
    using BinOpBase::BinOpBase;
};

struct Call : TwoChildrenBase<Expr, Exprs>, RExpr {
    LABEL("\"f(x)\"");
    using TwoChildrenBase::TwoChildrenBase;
};

//
// Statements
//

struct Statement : virtual Node {
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
struct ReturnValue : SingleChildBase<Expr>, Statement {
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

struct Block : MultiChildrenBase<Statement>, Statement {
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

struct ExpressionStmt : SingleChildBase<Expr>, Statement {
    LABEL("ExpressionStmt");
    using SingleChildBase::SingleChildBase;
};

struct Declaration : virtual Node {
    LABEL("Declaration");
};

struct FuncDef : SingleChildBase<Block>, Declaration {
    LABEL("FuncDef");
    const symtb::VarRef ref;

    FuncDef(Block* body, symtb::VarRef ref) : SingleChildBase(body), ref(ref) {}
};

struct Program : MultiChildrenBase<Declaration> {
    LABEL("Program");
};

}; // namespace ast

#endif /* AST_H */
