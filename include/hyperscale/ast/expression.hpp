/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <llvm/IR/Value.h>

namespace hyperscale {
namespace ast {

    /// Expression - Base class for all expression nodes.
    class Expression {
    public:
        virtual ~Expression() {}
        virtual llvm::Value *codegen() = 0;
    };

} // end of ast namespace
} // end of hyperscale namespace
