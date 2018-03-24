/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/expression.hpp>

namespace hyperscale {
namespace ast {

    /// NumberExpression - Expression class for numeric literals like "1.0".
    class NumberExpression : public Expression {
    double m_val;

    public:
        NumberExpression(double val) : m_val(val) {}

        virtual llvm::Value *codegen() = 0;
    };

} // end of ast namespace
} // end of hyperscale namespace
