/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/call-expr.hpp>

namespace hyperscale {
namespace ast {

    CallExpr::CallExpr(parser::Token& token):
        Expr(token),
        m_ref(token.getText().str()) {}

    // CallExpr::~CallExpr() {}

    std::string CallExpr::getRef() {
        return m_ref;
    }

    std::vector<Node*> CallExpr::getArguments() {
        return m_args;
    }

    void CallExpr::addArgument(Node* node) {
        m_args.push_back(node);
    }

    void CallExpr::accept(Visitor& visitor) {
        visitor(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace
