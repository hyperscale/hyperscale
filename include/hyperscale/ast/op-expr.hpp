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

    class OpExpr: public Expr {
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
        std::shared_ptr<Expr> m_left;

        Oper m_oper;

        std::shared_ptr<Expr> m_right;

    public:
        /// Construct an OpExpr node.
        OpExpr(std::shared_ptr<Expr> left, OpExpr::Oper oper, std::shared_ptr<Expr> right);

        OpExpr(const OpExpr&) = delete;

        OpExpr& operator=(const OpExpr&) = delete;

        virtual ~OpExpr();

        void accept(Visitor& v);

        inline std::shared_ptr<Expr> getLeft();

        inline OpExpr::Oper getOperator();

        inline std::shared_ptr<Expr> getRight();
    };

} // end of ast namespace
} // end of hyperscale namespace

#include <hyperscale/ast/op-expr.hxx>
