/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/paren-expr.hpp>

namespace hyperscale {
namespace ast {

    ParenExpr::ParenExpr(Expr* expr):
        Expr(),
        m_expr(expr) {}

    ParenExpr::~ParenExpr() {
        delete m_expr;
    }

    Expr* ParenExpr::getExpr() {
        return m_expr;
    }
/*
    void ParenExpr::accept(Visitor& visitor) {
        visitor(*this);
    }
*/
} // end of ast namespace
} // end of hyperscale namespace



