/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/op-expr.hpp>


namespace hyperscale {
namespace ast {

    OpExpr::OpExpr(Expr* left, OpExpr::Oper oper, Expr* right):
        Expr(),
        m_left(left),
        m_oper(oper),
        m_right(right) {}

    OpExpr::~OpExpr() {
        delete m_left;
        delete m_right;
    }

} // end of ast namespace
} // end of hyperscale namespace



