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
#include <hyperscale/ast/visitor.hpp>

namespace hyperscale {
namespace ast {

    template<class Derived>
    class Visitable {
    public:
        void visit(ConstVisitor& visitor) const;

        void visit(Visitor& visitor);
    };

} // end of ast namespace
} // end of hyperscale namespace


#include <hyperscale/ast/visitable.hxx>
