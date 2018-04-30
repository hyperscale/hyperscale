/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

// #include <hyperscale/ast/fwd.hpp>
#include <hyperscale/ast/pretty-printer-visitor.hpp>

namespace hyperscale {
namespace ast {

    PrettyPrinterVisitor::PrettyPrinterVisitor(std::ostream& ostr)
        : m_ostr(ostr),
          m_indent(0) {}

    PrettyPrinterVisitor::PrettyPrinterVisitor(std::ostream& ostr, std::size_t indent)
        : m_ostr(ostr),
          m_indent(indent) {}

    std::string PrettyPrinterVisitor::indent() {
        return indent(m_indent);
    }

    std::string PrettyPrinterVisitor::indent(std::size_t indent) {
        return std::string(indent, ' ');
    }
/*
    void PrettyPrinterVisitor::operator()(VarDecl& e) {
        m_ostr << "var(" << e.getName() << ")" << std::endl;
    }
*/

    void PrettyPrinterVisitor::operator()(ParenExpr& e) {
        m_ostr << indent() << "-ParenExpr <line:" << e.getLine() << ", col:" << e.getColumn() << ">" << std::endl;

        m_ostr << indent(2) << "`";
        e.getExpr()->accept(*this);
    }

    void PrettyPrinterVisitor::operator()(OpExpr& e) {
        m_ostr << indent() << "-BinaryOperator <line:" << e.getLine() << ", col:" << e.getColumn() << ">";
        m_ostr << " '" << e.getOperator() << "'" << std::endl;

        m_ostr << indent(2) << "|";
        e.getLeft()->accept(*this);

        m_ostr << indent(2) << "`";
        e.getRight()->accept(*this);

    /*
        PrettyPrinterVisitor print(m_ostr, m_indent + 2);

        print(*e.getLeft());
        print(*e.getRight());
    */
    }

    void PrettyPrinterVisitor::operator()(IntExpr& e) {
        m_ostr << indent() <<  "-IntegerLiteral <line:"<< e.getLine() << ", col:" << e.getColumn() << ">";
        m_ostr << " 'int' " << e.getValue() << std::endl;
    }

    void PrettyPrinterVisitor::operator()(Node& e) {
        m_ostr << "`";

        e.accept(*this);
    }

    std::ostream& operator<<(std::ostream& os, Node& tree) {
        PrettyPrinterVisitor print(os);

        print(tree);

        return os;
    }

} // end of ast namespace
} // end of hyperscale namespace



