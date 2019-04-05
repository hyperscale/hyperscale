/**
 * Hyperscale
 *
 * (c) 2015-2019 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <string>
#include <map>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace hyperscale {
namespace ir {

    class IRGeneratorModule {
    private:
        llvm::LLVMContext m_context;
        llvm::IRBuilder<> m_builder;
        std::unique_ptr<llvm::Module> m_module;
        std::unique_ptr<llvm::TargetMachine> m_target_machine;
        std::map<std::string, llvm::AllocaInst *> m_variables;

    public:
        IRGeneratorModule();

        llvm::LLVMContext getContext();

        llvm::IRBuilder<> getBuilder();

        std::unique_ptr<llvm::Module> getModule();

        void setVariable(const std::string &name, llvm::AllocaInst* value);

        bool hasVariable(const std::string &name);

        llvm::AllocaInst* getVariable(const std::string &name);

        void print();
    };

} // end of ir namespace
} // end of hyperscale namespace
