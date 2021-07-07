/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

// #include "lib/hyperscale/ast/fwd.hpp"
#include "lib/hyperscale/ast/visitor.hpp"
// #include "lib/hyperscale/ast/var-decl.hpp"
#include "lib/hyperscale/ast/int-expr.hpp"
#include "lib/hyperscale/ast/op-expr.hpp"
#include "lib/hyperscale/ast/paren-expr.hpp"
#include "lib/hyperscale/ast/var-decl.hpp"
#include "lib/hyperscale/ast/source-file.hpp"
#include "lib/hyperscale/ast/call-expr.hpp"
#include "lib/hyperscale/ast/decl-ref-expr.hpp"

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

        void operator()(Node&) override;
        void operator()(SourceFile&) override;
        void operator()(OpExpr& e) override;
        void operator()(IntExpr& e) override;
        void operator()(ParenExpr& e) override;
        void operator()(VarDecl& e) override;
        void operator()(CallExpr& e) override;
        void operator()(DeclRefExpr& e) override;
    };

} // end of ast namespace
} // end of hyperscale namespace
