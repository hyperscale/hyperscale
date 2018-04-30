/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/var-decl.hpp>

namespace hyperscale {
namespace ast {

    VarDecl::VarDecl(parser::Token& token, std::string type, Expr* value):
        Decl(token),
        m_type(type),
        m_value(value) {}

    VarDecl::~VarDecl() {
        delete m_value;
    }

    std::string VarDecl::getType() const {
        return m_type;
    }

    Expr* VarDecl::getValue() const {
        return m_value;
    }

    void VarDecl::accept(Visitor& visitor) {
        visitor(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace



