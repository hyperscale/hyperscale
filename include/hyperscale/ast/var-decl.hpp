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
#include <hyperscale/ast/decl.hpp>
#include <hyperscale/ast/expr.hpp>
#include <hyperscale/ast/visitable.hpp>

namespace hyperscale {
namespace ast {

    class VarDecl: public Decl, public Visitable<VarDecl> {
    protected:
        std::string m_type;
        bool m_const;
        Expr* m_value;

    public:
        /// Construct an VarDecl node.
        VarDecl(parser::Token& token, std::string type, Expr* value);

        VarDecl(const VarDecl&) = delete;

        VarDecl& operator=(const VarDecl&) = delete;

        ~VarDecl();

        std::string getType() const;

        Expr* getValue() const;

        // void accept(Visitor& visitor) override;
    };

} // end of ast namespace
} // end of hyperscale namespace
