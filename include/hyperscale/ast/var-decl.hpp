

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

namespace hyperscale {
namespace ast {

    class VarDecl : public Decl {
    protected:
        std::shared_ptr<NameTy> m_type;

        std::shared_ptr<Expr> m_value;

    public:
        /// Construct an OpExpr node.
        VarDecl(std::shared_ptr<parser::Token>& token, std::string name, std::shared_ptr<NameTy>& type, std::shared_ptr<Expr>& value):
            Decl(token, name),
            m_type(type),
            m_value(value) {}

        VarDecl(const VarDecl&) = delete;

        VarDecl& operator=(const VarDecl&) = delete;

        virtual ~VarDecl();

        std::shared_ptr<NameTy> getType() {
            return m_type;
        }

        std::shared_ptr<Expr> getValue() {
            return m_value;
        }

        void accept(Visitor& v) {
            v(*this);
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
