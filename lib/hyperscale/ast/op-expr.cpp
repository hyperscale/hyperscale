/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "lib/hyperscale/ast/op-expr.hpp"

namespace hyperscale {
namespace ast {

    OpExpr::OpExpr(Expr* left, Operator oper, Expr* right):
        Expr(),
        m_left(left),
        m_oper(oper),
        m_right(right) {}

    OpExpr::~OpExpr() {
        delete m_left;
        delete m_right;
    }

    Expr* OpExpr::getLeft() {
        return m_left;
    }

    Operator OpExpr::getOperator() {
        return m_oper;
    }

    Expr* OpExpr::getRight() {
        return m_right;
    }

    void OpExpr::accept(Visitor& visitor) {
        visitor(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace



