/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <memory>
#include <hyperscale/ast/expression.hpp>

namespace hyperscale {
namespace ast {

    /// BinaryExpression - Expression class for a binary operator.
    class BinaryExpression : public Expression {
        char m_operator;
        std::unique_ptr<Expression> LHS, RHS;

    public:
        BinaryExpression(char op, std::unique_ptr<Expression> LHS, std::unique_ptr<Expression> RHS)
            : m_operator(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

        virtual llvm::Value *codegen() = 0;
    };

} // end of ast namespace
} // end of hyperscale namespace

