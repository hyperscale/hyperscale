/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

// #include <hyperscale/ast/fwd.hpp>
#include <hyperscale/ast/graph-visitor.hpp>

namespace hyperscale {
namespace ast {

    GraphVisitor::GraphVisitor(std::ostream& ostr): m_ostr(ostr) {
        m_ostr << "digraph astgraph {" << std::endl;
        m_ostr << "  node [shape=circle, fontsize=12, fontname=\"Courier\", height=.1];" << std::endl;
        m_ostr << "  ranksep=.3;" << std::endl;
        m_ostr << "  edge [arrowsize=.5]" << std::endl;
    }

    GraphVisitor::~GraphVisitor() {
        m_ostr << "}" << std::endl;
    }

/*
    void PrettyPrinterVisitor::operator()(VarDecl& e) {
        m_ostr << "var(" << e.getName() << ")" << std::endl;
    }
*/

    void GraphVisitor::operator()(ParenExpr& e) {
        e.getExpr()->accept(*this);
    }

    void GraphVisitor::operator()(OpExpr& e) {
        m_ostr << "  node" << static_cast<void*>(&e) << " [label=\"" << e.getOperator() << "\"];" << std::endl;

        e.getLeft()->accept(*this);
        e.getRight()->accept(*this);

        m_ostr << "  node" << static_cast<void*>(&e) << " -> node" << static_cast<void*>(e.getLeft()) << ";" << std::endl;
        m_ostr << "  node" << static_cast<void*>(&e) << " -> node" << static_cast<void*>(e.getRight()) << ";" << std::endl;
    }

    void GraphVisitor::operator()(IntExpr& e) {
        m_ostr <<  "  node" << static_cast<void*>(&e) << " [label=\"" << e.getValue() << "\"];" << std::endl;
    }

    void GraphVisitor::operator()(Node& e) {
        e.accept(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace


