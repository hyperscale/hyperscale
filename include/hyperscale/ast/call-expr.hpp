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
#include <vector>
#include <hyperscale/ast/expr.hpp>

namespace hyperscale {
namespace ast {

    class CallExpr: public Expr {
    protected:
        std::string m_ref;

        std::vector<Node*> m_args;

    public:
        /// Construct an CallExpr node.
        CallExpr(parser::Token& token);

        CallExpr(const CallExpr&) = delete;

        CallExpr& operator=(const CallExpr&) = delete;

        ~CallExpr();

        void addArgument(Node* node);

        std::vector<Node*> getArguments();

        std::string getRef();

        void accept(Visitor& visitor) override;
    };

} // end of ast namespace
} // end of hyperscale namespace
