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

    VarDecl::VarDecl(parser::Token& token):
        Decl(token) {}

    VarDecl::VarDecl(parser::Token& token, std::string type, Expr* value):
        Decl(token),
        m_type(type),
        m_value(value) {}

    VarDecl::~VarDecl() {
        delete m_value;
    }

    void VarDecl::setType(std::string type) {
        m_type = type;
    }

    std::string VarDecl::getType() const {
        return m_type;
    }

    void VarDecl::setValue(Expr* value) {
        m_value = value;
    }

    Expr* VarDecl::getValue() const {
        return m_value;
    }

    void VarDecl::accept(Visitor& visitor) {
        visitor(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace



