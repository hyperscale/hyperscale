/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <vector>
#include <memory>
#include <hyperscale/ast/expression.hpp>

namespace hyperscale {
namespace ast {

    /// CallExpression - Expression class for function calls.
    class CallExpression : public Expression {
        std::string m_callee;
        std::vector<Expression*> m_args;

    public:
        CallExpression(const std::string &callee, std::vector<Expression*> args)
            : m_callee(callee), m_args(args) {}

        virtual llvm::Value *codegen() = 0;
    };

} // end of ast namespace
} // end of hyperscale namespace

