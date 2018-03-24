/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/node.hpp>

namespace hyperscale {
namespace ast {

    class Expr: public Node
    {
    public:
        Expr():
            Node() {}

        Expr(std::shared_ptr<parser::Token>& token):
            Node(token) {}

        Expr(const Expr&) = delete;

        Expr& operator=(const Expr&) = delete;

        virtual ~Expr() = default;
    };

} // end of ast namespace
} // end of hyperscale namespace
