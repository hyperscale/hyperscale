/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <string>
#include <hyperscale/ast/fwd.hpp>

namespace hyperscale {
namespace ast {

    class Visitor {
    public:
        virtual ~Visitor();

        virtual void operator()(OpExpr*) = 0;
        virtual void operator()(IntExpr*) = 0;
        virtual void operator()(VarDecl*) = 0;

        /// Helper to visit nodes manipulated via a pointer.
        template <class E>
        void operator()(E* e);

    protected:
        template <typename E>
        void accept(E* e);
    };

} // end of ast namespace
} // end of hyperscale namespace


