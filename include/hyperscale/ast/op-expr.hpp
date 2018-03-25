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
#include <hyperscale/ast/visitable.hpp>

namespace hyperscale {
namespace ast {

    class OpExpr: public Expr, public Visitable<OpExpr>  {
    public:
        /// Operator qualifier.
        enum class Oper {
            // Arithmetics.
            /** \brief "+" */ add,
            /** \brief "-" */ sub,
            /** \brief "*" */ mul,
            /** \brief "/" */ div,

            // Comparison.
            /** \brief "=" */  eq,
            /** \brief "<>" */ ne,
            /** \brief "<" */  lt,
            /** \brief "<=" */ le,
            /** \brief ">" */  gt,
            /** \brief ">=" */ ge
        };

    protected:
        Expr* m_left;

        Oper m_oper;

        Expr* m_right;

    public:
        /// Construct an OpExpr node.
        OpExpr(Expr* left, OpExpr::Oper oper, Expr* right);

        OpExpr(const OpExpr&) = delete;

        OpExpr& operator=(const OpExpr&) = delete;

        virtual ~OpExpr();

        inline Expr* getLeft();

        inline OpExpr::Oper getOperator();

        inline Expr* getRight();
    };

} // end of ast namespace
} // end of hyperscale namespace

#include <hyperscale/ast/op-expr.hxx>
