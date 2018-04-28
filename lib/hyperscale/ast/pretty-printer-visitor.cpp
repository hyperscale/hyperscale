/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/fwd.hpp>
#include <hyperscale/ast/pretty-printer-visitor.hpp>

namespace hyperscale {
namespace ast {

    PrettyPrinterVisitor::PrettyPrinterVisitor(std::ostream& ostr)
        : m_ostr(ostr) {}

    void PrettyPrinterVisitor::operator()(VarDecl& e) {
        m_ostr << "var(" << e.getName() << ")" << std::endl;
    }

    void PrettyPrinterVisitor::operator()(OpExpr& e) {
        m_ostr << "operator(" << e.getOperator() << ")" << std::endl;
    }

    void PrettyPrinterVisitor::operator()(IntExpr& e) {
        m_ostr << "int(" << e.getValue() << ")" << std::endl;
    }


    std::ostream& operator<<(std::ostream& os, Node& tree) {
        PrettyPrinterVisitor print(os);

        print(tree);

        return os;
    }

} // end of ast namespace
} // end of hyperscale namespace



