/**
 * Hyperscale
 *
 * (c) 2015-2019 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ir/ir-generator-module.hpp>

namespace hyperscale {
namespace ir {

    IRGeneratorModule::IRGeneratorModule(): m_builder(m_context) {
        m_module = llvm::make_unique<llvm::Module>("main", m_context);
    }

    llvm::LLVMContext IRGeneratorModule::getContext() {
        return m_context;
    }

    llvm::IRBuilder<> IRGeneratorModule::getBuilder() {
        return m_builder;
    }

    std::unique_ptr<llvm::Module> IRGeneratorModule::getModule() {
        return std::move(m_module);
    }

    void IRGeneratorModule::setVariable(const std::string &name, llvm::AllocaInst* value) {
        m_variable[name] = value;
    }

    bool IRGeneratorModule::hasVariable(const std::string &name) {
        return m_variable.count(name) > 0;
    }

    llvm::AllocaInst* IRGeneratorModule::getVariable(const std::string &name) {
        //@TODO: check if exists before get
        return m_variable[name];
    }

    void IRGeneratorModule::print() {
        m_module->print(llvm::errs(), nullptr);
    }

} // end of ir namespace
} // end of hyperscale namespace
