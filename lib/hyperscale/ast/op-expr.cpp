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

    OpExpr::OpExpr(std::shared_ptr<Expr> left, OpExpr::Oper oper, std::shared_ptr<Expr> right):
        Expr(),
        m_left(left),
        m_oper(oper),
        m_right(right) {}

    OpExpr::~OpExpr() {}

    void OpExpr::accept(Visitor& v) {
        v(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace



