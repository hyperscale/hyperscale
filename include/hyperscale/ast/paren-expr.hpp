/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/expr.hpp>
#include <hyperscale/ast/visitor.hpp>

namespace hyperscale {
namespace ast {

    class ParenExpr: public Expr {
    protected:
        Expr* m_expr;

    public:
        /// Construct an ParenExpr node.
        ParenExpr(Expr* expr);

        ParenExpr(const ParenExpr&) = delete;

        ParenExpr& operator=(const ParenExpr&) = delete;

        ~ParenExpr();

        Expr* getExpr();

        void accept(Visitor& visitor) override;
    };

} // end of ast namespace
} // end of hyperscale namespace

