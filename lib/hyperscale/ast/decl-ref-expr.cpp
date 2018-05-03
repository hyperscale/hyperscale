/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/decl-ref-expr.hpp>

namespace hyperscale {
namespace ast {

    DeclRefExpr::DeclRefExpr(parser::Token& token):
        Expr(token),
        m_ref(token.getText()) {}

    // DeclRefExpr::~DeclRefExpr() {}

    std::string DeclRefExpr::getRef() {
        return m_ref;
    }

    void DeclRefExpr::accept(Visitor& visitor) {
        visitor(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace
