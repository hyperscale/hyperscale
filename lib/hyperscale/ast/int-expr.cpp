/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/int-expr.hpp>

namespace hyperscale {
namespace ast {

    IntExpr::IntExpr(parser::Token& token):
        Expr(token),
        m_value(std::stoi(token.getText())) {}

    // IntExpr::~IntExpr() {}

    int IntExpr::getValue() const {
        return m_value;
    }

    void IntExpr::accept(Visitor& visitor) {
        visitor(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace
