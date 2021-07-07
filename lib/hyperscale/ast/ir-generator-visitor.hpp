/**
 * Hyperscale
 *
 * (c) 2015-2019 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <iostream>
#include "lib/hyperscale/ast/default-visitor.hpp"
#include "lib/hyperscale/ir/ir-generator-module.hpp"
#include <llvm/Support/Error.h>
#include <llvm/ADT/APFloat.h>

namespace hyperscale {
namespace ast {

    class IRGeneratorVisitor: virtual public DefaultVisitor {
    protected:
        ir::IRGeneratorModule& m_module;
        llvm::Value* m_value;

    public:
        using super_type = DefaultVisitor;
        // Import overloaded virtual functions.
        using super_type::operator();

        /// Build IR
        IRGeneratorVisitor(ir::IRGeneratorModule& module);

        void operator()(SourceFile& e);

        void operator()(VarDecl& e);

        void operator()(Node&);

        void operator()(OpExpr& e);

        void operator()(IntExpr& e);

        void operator()(ParenExpr& e);

        void operator()(CallExpr& e);

        void operator()(DeclRefExpr& e);

        llvm::Value* getValue();
    };

    //std::ostream& operator<<(std::ostream& os, Node& tree);

} // end of ast namespace
} // end of hyperscale namespace


