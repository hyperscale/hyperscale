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
#include <hyperscale/ast/visitor.hpp>

namespace hyperscale {
namespace ast {

    class Visitor;

    template<class Derived>
    class Visitable {
    public:
        void accept(Visitor& visitor);
    };

} // end of ast namespace
} // end of hyperscale namespace


#include <hyperscale/ast/visitable.hxx>
