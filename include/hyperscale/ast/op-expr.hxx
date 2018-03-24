/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/op-expr.hpp>

namespace hyperscale {
namespace ast {

    std::shared_ptr<Expr> OpExpr::getLeft() {
        return m_left;
    }

    OpExpr::Oper OpExpr::getOperator() {
        return m_oper;
    }

    std::shared_ptr<Expr> OpExpr::getRight() {
        return m_right;
    }


} // end of ast namespace
} // end of hyperscale namespace
