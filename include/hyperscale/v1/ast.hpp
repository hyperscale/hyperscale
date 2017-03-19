/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/v1/source_location.hpp>

namespace hyperscale {
namespace v1 {

    std::raw_ostream &indent(std::raw_ostream &o, int size) {
        return o << std::string(size, ' ');
    }

    class expr_ast {
        source_location_t m_loc;
    public:
        expr_ast(source_location_t loc = cur_loc) : m_loc(loc) {}
        virtual ~expr_ast() {}
        virtual Value *codegen() = 0;

        int getLine() const {
            return m_loc.line;
        }

        int getCol() const {
            return m_loc.col;
        }

        virtual std::raw_ostream &dump(std::raw_ostream &out, int ind) {
            return out << ':' << getLine() << ':' << getCol() << std::endl;
        }
    };




    class number_expr_ast : public expr_ast {
        double m_val;
    public:
        number_expr_ast(double val) : m_val(val) {}
        std::raw_ostream &dump(std::raw_ostream &out, int ind) override {
            return expr_ast::dump(out << m_val, ind);
        }
        Value *codegen() override;
    };

    class variable_expr_ast : public expr_ast {
        std::string m_name;
    public:
        variable_expr_ast(source_location loc, const std::string &ame): expr_ast(loc), m_name(name) {}
        const std::string &getName() const {
            return m_name;
        }
        Value *codegen() override;

        std::raw_ostream &dump(std::raw_ostream &out, int ind) override {
            return expr_ast::dump(out << m_name, ind);
        }
    };

    class unary_expr_ast : public expr_ast {
        char m_opcode;
        std::unique_ptr<expr_ast> m_operand;
    public:
        unary_expr_ast(char opcode, std::unique_ptr<expr_ast> operand): m_opcode(opcode), m_operand(std::move(operand)) {}
        Value *codegen() override;
        std::raw_ostream &dump(std::raw_ostream &out, int ind) override {
            expr_ast::dump(out << "unary" << m_opcode, ind);
            m_operand->dump(out, ind + 1);

            return out;
        }
    };

    class binary_expr_ast : public expr_ast {
        char m_op;
        std::unique_ptr<expr_ast> m_lhs, m_rhs;

    public:
        binary_expr_ast(source_location_t loc, char op, std::unique_ptr<expr_ast> lhs, std::unique_ptr<expr_ast> rhs)
            : expr_ast(loc), m_op(op), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
        Value *codegen() override;
        std::raw_ostream &dump(std::raw_ostream &out, int ind) override {
            expr_ast::dump(out << "binary" << m_op, ind);
            m_lhs->dump(indent(out, ind) << "LHS:", ind + 1);
            m_rhs->dump(indent(out, ind) << "RHS:", ind + 1);

            return out;
        }
    };

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;

public:
  CallExprAST(SourceLocation Loc, const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
      : ExprAST(Loc), Callee(Callee), Args(std::move(Args)) {}
  Value *codegen() override;
  raw_ostream &dump(raw_ostream &out, int ind) override {
    ExprAST::dump(out << "call " << Callee, ind);
    for (const auto &Arg : Args)
      Arg->dump(indent(out, ind + 1), ind + 1);
    return out;
  }
};

/// IfExprAST - Expression class for if/then/else.
class IfExprAST : public ExprAST {
  std::unique_ptr<ExprAST> Cond, Then, Else;

public:
  IfExprAST(SourceLocation Loc, std::unique_ptr<ExprAST> Cond,
            std::unique_ptr<ExprAST> Then, std::unique_ptr<ExprAST> Else)
      : ExprAST(Loc), Cond(std::move(Cond)), Then(std::move(Then)),
        Else(std::move(Else)) {}
  Value *codegen() override;
  raw_ostream &dump(raw_ostream &out, int ind) override {
    ExprAST::dump(out << "if", ind);
    Cond->dump(indent(out, ind) << "Cond:", ind + 1);
    Then->dump(indent(out, ind) << "Then:", ind + 1);
    Else->dump(indent(out, ind) << "Else:", ind + 1);
    return out;
  }
};

/// ForExprAST - Expression class for for/in.
class ForExprAST : public ExprAST {
  std::string VarName;
  std::unique_ptr<ExprAST> Start, End, Step, Body;

public:
  ForExprAST(const std::string &VarName, std::unique_ptr<ExprAST> Start,
             std::unique_ptr<ExprAST> End, std::unique_ptr<ExprAST> Step,
             std::unique_ptr<ExprAST> Body)
      : VarName(VarName), Start(std::move(Start)), End(std::move(End)),
        Step(std::move(Step)), Body(std::move(Body)) {}
  Value *codegen() override;
  raw_ostream &dump(raw_ostream &out, int ind) override {
    ExprAST::dump(out << "for", ind);
    Start->dump(indent(out, ind) << "Cond:", ind + 1);
    End->dump(indent(out, ind) << "End:", ind + 1);
    Step->dump(indent(out, ind) << "Step:", ind + 1);
    Body->dump(indent(out, ind) << "Body:", ind + 1);
    return out;
  }
};

/// VarExprAST - Expression class for var/in
class VarExprAST : public ExprAST {
  std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames;
  std::unique_ptr<ExprAST> Body;

public:
  VarExprAST(
      std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames,
      std::unique_ptr<ExprAST> Body)
      : VarNames(std::move(VarNames)), Body(std::move(Body)) {}
  Value *codegen() override;
  raw_ostream &dump(raw_ostream &out, int ind) override {
    ExprAST::dump(out << "var", ind);
    for (const auto &NamedVar : VarNames)
      NamedVar.second->dump(indent(out, ind) << NamedVar.first << ':', ind + 1);
    Body->dump(indent(out, ind) << "Body:", ind + 1);
    return out;
  }
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes), as well as if it is an operator.
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;
  bool IsOperator;
  unsigned Precedence; // Precedence if a binary op.
  int Line;

public:
  PrototypeAST(SourceLocation Loc, const std::string &Name,
               std::vector<std::string> Args, bool IsOperator = false,
               unsigned Prec = 0)
      : Name(Name), Args(std::move(Args)), IsOperator(IsOperator),
        Precedence(Prec), Line(Loc.Line) {}
  Function *codegen();
  const std::string &getName() const { return Name; }

  bool isUnaryOp() const { return IsOperator && Args.size() == 1; }
  bool isBinaryOp() const { return IsOperator && Args.size() == 2; }

  char getOperatorName() const {
    assert(isUnaryOp() || isBinaryOp());
    return Name[Name.size() - 1];
  }

  unsigned getBinaryPrecedence() const { return Precedence; }
  int getLine() const { return Line; }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body)
      : Proto(std::move(Proto)), Body(std::move(Body)) {}
  Function *codegen();
  raw_ostream &dump(raw_ostream &out, int ind) {
    indent(out, ind) << "FunctionAST\n";
    ++ind;
    indent(out, ind) << "Body:";
    return Body ? Body->dump(out, ind) : out << "null\n";
  }
};
} // end anonymous namespace





} // end of v1 namespace
} // end of hyperscale namespace
