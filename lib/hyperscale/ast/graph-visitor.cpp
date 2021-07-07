/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "lib/hyperscale/ast/graph-visitor.hpp"

namespace hyperscale {
namespace ast {

    GraphVisitor::GraphVisitor(std::ostream& ostr): m_ostr(ostr) {
        m_ostr << "digraph astgraph {" << std::endl;
        m_ostr << "  node [shape=plaintext, fontsize=12, fontname=\"Courier\", height=.1];" << std::endl;
        m_ostr << "  ranksep=.3;" << std::endl;
        m_ostr << "  edge [arrowsize=.5]" << std::endl;
    }

    GraphVisitor::~GraphVisitor() {
        m_ostr << "}" << std::endl;
    }

    void GraphVisitor::operator()(VarDecl& e) {
        m_ostr << "  node" << static_cast<void*>(&e) << " [label=\"" << e.getType() << "\"];" << std::endl;

        e.getValue()->accept(*this);

        m_ostr << "  node" << static_cast<void*>(&e) << " -> node" << static_cast<void*>(e.getValue()) << ";" << std::endl;
    }

    void GraphVisitor::operator()(ParenExpr& e) {
        m_ostr << "  node" << static_cast<void*>(&e) << " [label=\"factor\"];" << std::endl;

        m_ostr << "  node" << static_cast<void*>(&e) << "_open [label=\"(\"];" << std::endl;

        e.getExpr()->accept(*this);

        m_ostr << "  node" << static_cast<void*>(&e) << "_close [label=\")\"];" << std::endl;

        m_ostr << "  node" << static_cast<void*>(&e) << " -> node" << static_cast<void*>(&e) << "_open" << ";" << std::endl;
        m_ostr << "  node" << static_cast<void*>(&e) << " -> node" << static_cast<void*>(e.getExpr()) << ";" << std::endl;
        m_ostr << "  node" << static_cast<void*>(&e) << " -> node" << static_cast<void*>(&e) << "_close" << ";" << std::endl;
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

    void GraphVisitor::operator()(SourceFile& e) {
        m_ostr << "  node" << static_cast<void*>(&e) << " [label=\"file\"];" << std::endl;

        for (auto decl = e.getDecls().begin(); decl != e.getDecls().end(); ++decl) {
            (*decl)->accept(*this);

            m_ostr << "  node" << static_cast<void*>(&e) << " -> node" << static_cast<void*>((*decl)) << ";" << std::endl;
        }
    }

    void GraphVisitor::operator()(DeclRefExpr& e) {
        m_ostr << "  node" << static_cast<void*>(&e) << " [label=\"" << e.getRef() << "\"];" << std::endl;
    }

    void GraphVisitor::operator()(CallExpr& e) {
        m_ostr << "  node" << static_cast<void*>(&e) << " [label=\"" << e.getRef() << "\"];" << std::endl;

        for (auto node = e.getArguments().begin(); node != e.getArguments().end(); ++node) {
            (*node)->accept(*this);

            m_ostr << "  node" << static_cast<void*>(&e) << " -> node" << static_cast<void*>((*node)) << ";" << std::endl;
        }
    }

    void GraphVisitor::operator()(Node& e) {
        e.accept(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace



