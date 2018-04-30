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
#include <hyperscale/ast/operator.hpp>

namespace hyperscale {
namespace ast {

    class OpExpr: public Expr {
    protected:
        Expr* m_left;

        Operator m_oper;

        Expr* m_right;

    public:
        /// Construct an OpExpr node.
        OpExpr(Expr* left, Operator oper, Expr* right);

        OpExpr(const OpExpr&) = delete;

        OpExpr& operator=(const OpExpr&) = delete;

        ~OpExpr();

        Expr* getLeft();

        Operator getOperator();

        Expr* getRight();

        void accept(Visitor& visitor) override;
    };

} // end of ast namespace
} // end of hyperscale namespace

