/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <string>
#include <hyperscale/ast/expression.hpp>

namespace hyperscale {
namespace ast {

    /// VariableExpression - Expression class for referencing a variable, like "a".
    class VariableExpression : public Expression {
        std::string m_name;

    public:
        VariableExpression(const std::string &name) : m_name(name) {}

        virtual llvm::Value *codegen() = 0;
    };

} // end of ast namespace
} // end of hyperscale namespace


