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
#include <hyperscale/ast/visitable.hpp>

namespace hyperscale {
namespace ast {

    class DeclRefExpr: public Expr, public Visitable<DeclRefExpr> {
    protected:
        std::string m_ref;

    public:
        /// Construct an DeclRefExpr node.
        DeclRefExpr(parser::Token& token);

        DeclRefExpr(const DeclRefExpr&) = delete;

        DeclRefExpr& operator=(const DeclRefExpr&) = delete;

        virtual ~DeclRefExpr() = default;

        std::string getRef();

        //void accept(Visitor& visitor) override;
    };

} // end of ast namespace
} // end of hyperscale namespace
