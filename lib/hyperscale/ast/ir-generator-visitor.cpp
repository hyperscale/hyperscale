/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/ir-generator-visitor.hpp>

namespace hyperscale {
namespace ast {

    IRGeneratorVisitor::IRGeneratorVisitor(ir::IRGeneratorModule& module)
        : m_module(module) {}

    llvm::Value* IRGeneratorVisitor::getValue() {
        return m_value;
    }

    void IRGeneratorVisitor::operator()(VarDecl& e) {
        auto alloca = m_module.getBuilder().CreateAlloca(
            llvm::Type::getDoubleTy(m_module.getContext()),
            0,
            e.getType().c_str()
        );

        m_module.setVariable(e.getType(), alloca);

        IRGeneratorVisitor visit(m_module);

        visit(*e.getValue());

        auto expr = visit.getValue();

        m_module.getBuilder().CreateStore(visit.getValue(), alloca);
    }


    void IRGeneratorVisitor::operator()(ParenExpr& e) {
        /*m_ostr << indent() << "ParenExpr <line:" << e.getLine() << ", col:" << e.getColumn() << ">" << std::endl;

        IRGeneratorVisitor print(m_ostr, m_indent+1);

        print(*e.getExpr());*/
    }

    void IRGeneratorVisitor::operator()(OpExpr& e) {
        IRGeneratorVisitor visit(m_module);

        visit(*e.getLeft());

        auto L = visit.getValue();

        visit(*e.getRight());

        auto R = visit.getValue();

        switch (e.getOperator()) {
        case Operator::add:
            m_value = m_module.getBuilder().CreateFAdd(L, R, "addtmp");
        case Operator::sub:
            m_value = m_module.getBuilder().CreateFSub(L, R, "subtmp");
        case Operator::mul:
            m_value = m_module.getBuilder().CreateFMul(L, R, "multmp");
        case Operator::lt:
            L = m_module.getBuilder().CreateFCmpULT(L, R, "cmptmp");
            // Convert bool 0/1 to double 0.0 or 1.0
            m_value = m_module.getBuilder().CreateUIToFP(L, llvm::Type::getDoubleTy(m_module.getContext()), "booltmp");
        default:
            std::cout << "invalid binary operator" << std::endl;

            m_value = nullptr;
        }
    }

    void IRGeneratorVisitor::operator()(IntExpr& e) {
        m_value = llvm::ConstantFP::get(m_module.getContext(), llvm::APFloat((float)(e.getValue())));
    }

    void IRGeneratorVisitor::operator()(SourceFile& e) {
        IRGeneratorVisitor visit(m_module);

        for (auto decl = e.getDecls().begin(); decl != e.getDecls().end(); ++decl) {
            visit(**decl);
        }
    }

    void IRGeneratorVisitor::operator()(CallExpr& e) {
        /*m_ostr << indent() <<  "CallExpr <line:" << e.getLine() << ", col:" << e.getColumn() << "> '" << e.getRef() << "'" << std::endl;

        IRGeneratorVisitor print(m_ostr, m_indent+1);

        for (auto node = e.getArguments().begin(); node != e.getArguments().end(); ++node) {
            print(**node);
        }*/
    }

    void IRGeneratorVisitor::operator()(DeclRefExpr& e) {
        //m_ostr << indent() <<  "DeclRefExpr <line:" << e.getLine() << ", col:" << e.getColumn() << "> '" << e.getRef() << "'" << std::endl;
    }

    void IRGeneratorVisitor::operator()(Node& e) {
        e.accept(*this);
    }

/*
    std::ostream& operator<<(std::ostream& os, Node& tree) {
        PrettyPrinterVisitor print(os);

        print(tree);

        return os;
    }

*/

} // end of ast namespace
} // end of hyperscale namespace



