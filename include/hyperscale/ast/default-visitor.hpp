/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/fwd.hpp>
#include <hyperscale/ast/visitor.hpp>
#include <hyperscale/ast/var-decl.hpp>
#include <hyperscale/ast/int-expr.hpp>

namespace hyperscale {
namespace ast {

    class DefaultVisitor: public virtual Visitor {
    public:
        /// Super class type.
        using super_type = Visitor;

        // Import overloaded \c operator() methods.
        using super_type::operator();

        /// Construct a default visitor.
        DefaultVisitor();

        /// Destroy a default visitor.
        virtual ~DefaultVisitor();

        void operator()(OpExpr& e) override;
        void operator()(IntExpr& e) override;
        void operator()(VarDecl& e) override;

    };

} // end of ast namespace
} // end of hyperscale namespace
