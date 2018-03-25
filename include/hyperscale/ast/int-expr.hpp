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
#include <hyperscale/ast/expr.hpp>
#include <hyperscale/ast/visitor.hpp>
#include <hyperscale/ast/visitable.hpp>

namespace hyperscale {
namespace ast {

    class IntExpr: public Expr, public Visitable<IntExpr> {
    protected:
        int m_value;

    public:
        /// Construct an IntExpr node.
        IntExpr(parser::Token& token);

        IntExpr(const IntExpr&) = delete;

        IntExpr& operator=(const IntExpr&) = delete;

        virtual ~IntExpr();

        inline int getValue();
    };

} // end of ast namespace
} // end of hyperscale namespace

#include <hyperscale/ast/int-expr.hxx>
