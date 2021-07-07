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

#include "lib/hyperscale/ast/expr.hpp"

namespace hyperscale {
namespace ast {

    class IntExpr: public Expr {
    protected:
        int m_value;

    public:
        /// Construct an IntExpr node.
        IntExpr(parser::Token& token);

        IntExpr(const IntExpr&) = delete;

        IntExpr& operator=(const IntExpr&) = delete;

        virtual ~IntExpr() = default;

        int getValue() const;

        void accept(Visitor& visitor) override;
    };

} // end of ast namespace
} // end of hyperscale namespace
