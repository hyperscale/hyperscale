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
#include "llvm/IR/LLVMContext.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"

namespace hyperscale {
namespace ir {

    class IRGeneratorModule {
    public:
        llvm::LLVMContext context;
        std::unique_ptr<llvm::Module> module;
        std::unique_ptr<llvm::IRBuilder<>> builder;
        std::unique_ptr<llvm::TargetMachine> target_machine;
        std::map<std::string, llvm::AllocaInst *> variablesAlloc;
        std::map<std::string, llvm::Value *> variables;

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
            module->setSourceFileName("main.hps");

            llvm::FunctionType *FuncType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);

            llvm::Function *func = llvm::Function::Create(FuncType, llvm::Function::ExternalLinkage, "main", module.get());

            llvm::BasicBlock *BB = llvm::BasicBlock::Create(context, "entry", func);

            builder->SetInsertPoint(BB);
        }

        /*llvm::LLVMContext getContext();

        llvm::IRBuilder<> getBuilder();

        std::unique_ptr<llvm::Module> getModule();*/

        void setVariableAlloc(const std::string &name, llvm::AllocaInst* value);

        bool hasVariableAlloc(const std::string &name);

        llvm::AllocaInst* getVariableAlloc(const std::string &name);

        void setVariable(const std::string &name, llvm::Value* value);

        bool hasVariable(const std::string &name);

        llvm::Value* getVariable(const std::string &name);

        void print();
    };

} // end of ir namespace
} // end of hyperscale namespace
