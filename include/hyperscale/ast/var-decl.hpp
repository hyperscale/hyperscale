

/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/fwd.hpp>
#include <hyperscale/ast/expr.hpp>
#include <hyperscale/ast/decl.hpp>
#include <hyperscale/ast/name-ty.hpp>
#include <hyperscale/ast/visitor.hpp>
#include <hyperscale/ast/visitable.hpp>

namespace hyperscale {
namespace ast {

    class VarDecl: public Decl, public Visitable<VarDecl>  {
    protected:
        NameTy* m_type;

        Expr* m_value;

    public:
        /// Construct an OpExpr node.
        VarDecl(parser::Token& token, std::string name, NameTy* type, Expr* value):
            Decl(token, name),
            m_type(type),
            m_value(value) {}

        VarDecl(const VarDecl&) = delete;

        VarDecl& operator=(const VarDecl&) = delete;

        virtual ~VarDecl();

        NameTy* getType() {
            return m_type;
        }

        Expr* getValue() {
            return m_value;
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
