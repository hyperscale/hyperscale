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
#include <llvm/Support/TargetSelect.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace hyperscale {
namespace ir {

    class IRGeneratorModule {
    public:
        llvm::LLVMContext context;
        std::unique_ptr<llvm::Module> module;
        std::unique_ptr<llvm::IRBuilder<>> builder;
        std::unique_ptr<llvm::TargetMachine> target_machine;
        std::map<std::string, llvm::AllocaInst *> variables;

    public:
        IRGeneratorModule():
            module(new llvm::Module("main", context)),
            builder(new llvm::IRBuilder<>(context))
        {
            /*
            llvm::InitializeNativeTarget();
            llvm::InitializeNativeTargetAsmPrinter();
            llvm::InitializeNativeTargetAsmParser();
            */
        }

        /*llvm::LLVMContext getContext();

        llvm::IRBuilder<> getBuilder();

        std::unique_ptr<llvm::Module> getModule();*/

        void setVariable(const std::string &name, llvm::AllocaInst* value);

        bool hasVariable(const std::string &name);

        llvm::AllocaInst* getVariable(const std::string &name);

        void print();
    };

} // end of ir namespace
} // end of hyperscale namespace
