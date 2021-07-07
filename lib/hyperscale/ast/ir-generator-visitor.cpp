/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "lib/hyperscale/ast/ir-generator-visitor.hpp"

namespace hyperscale {
namespace ast {

    IRGeneratorVisitor::IRGeneratorVisitor(ir::IRGeneratorModule& module)
        : m_module(module) {}

    llvm::Value* IRGeneratorVisitor::getValue() {
        return m_value;
    }

    void IRGeneratorVisitor::operator()(VarDecl& e) {
        auto val = e.getType().c_str();

        auto alloca = m_module.builder->CreateAlloca(
            m_module.builder->getDoubleTy(),
            0,
            val
        );

        m_module.setVariableAlloc(e.getType(), alloca);

        IRGeneratorVisitor visit(m_module);

        visit(*e.getValue());

        auto value = visit.getValue();

        m_module.setVariable(e.getType(), value);

        m_module.builder->CreateStore(value, alloca);
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
            m_value = m_module.builder->CreateFAdd(L, R, "addtmp");
            break;
        case Operator::sub:
            m_value = m_module.builder->CreateFSub(L, R, "subtmp");
            break;
        case Operator::mul:
            m_value = m_module.builder->CreateFMul(L, R, "multmp");
            break;
        case Operator::lt:
            L = m_module.builder->CreateFCmpULT(L, R, "cmptmp");
            // Convert bool 0/1 to double 0.0 or 1.0
            m_value = m_module.builder->CreateUIToFP(L, llvm::Type::getDoubleTy(m_module.context), "booltmp");
            break;
        default:
            //@TODO: use error manager
            std::cout << "invalid binary operator: " << e.getOperator() << std::endl;

            m_value = nullptr;
        }
    }

    void IRGeneratorVisitor::operator()(IntExpr& e) {
        m_value = llvm::ConstantFP::get(m_module.context, llvm::APFloat((double)(e.getValue())));
    }

    void IRGeneratorVisitor::operator()(SourceFile& e) {
        IRGeneratorVisitor visit(m_module);

        for (auto decl = e.getDecls().begin(); decl != e.getDecls().end(); ++decl) {
            visit(**decl);
        }
    }

    void IRGeneratorVisitor::operator()(CallExpr& e) {
        //@TODO: for func declaration use method of m_module.getFunc(e.getRef())
        auto *callee = m_module.module->getOrInsertFunction(
            e.getRef(),
            llvm::FunctionType::get(
                llvm::IntegerType::getInt32Ty(m_module.context),
                llvm::PointerType::get(llvm::Type::getInt8Ty(m_module.context), 0),
                true /* this is var arg func type*/
            )
        );

        /*m_ostr << indent() <<  "CallExpr <line:" << e.getLine() << ", col:" << e.getColumn() << "> '" << e.getRef() << "'" << std::endl;

        IRGeneratorVisitor print(m_ostr, m_indent+1);

        for (auto node = e.getArguments().begin(); node != e.getArguments().end(); ++node) {
            print(**node);
        }*/


        std::vector<llvm::Value *> args;

        for (auto node = e.getArguments().begin(); node != e.getArguments().end(); ++node) {
            IRGeneratorVisitor visit(m_module);

            visit(**node);

            args.push_back(visit.getValue());
        }

        m_value = m_module.builder->CreateCall(callee, args, e.getRef() + "Call");
    }

    void IRGeneratorVisitor::operator()(DeclRefExpr& e) {
        //m_ostr << indent() <<  "DeclRefExpr <line:" << e.getLine() << ", col:" << e.getColumn() << "> '" << e.getRef() << "'" << std::endl;

        auto value = m_module.getVariable(e.getRef());
        if (value == nullptr) {
            std::cout << "undefined variable " <<  e.getRef() << std::endl;

            return;
            // throw undefined variable e.getRef()
        }

        m_value = value;
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



