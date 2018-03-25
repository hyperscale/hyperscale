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
#include <hyperscale/ast/fwd.hpp>
#include <hyperscale/ast/ty.hpp>
#include <hyperscale/ast/visitor.hpp>
#include <hyperscale/ast/visitable.hpp>

namespace hyperscale {
namespace ast {

    class NameTy: public Ty, public Visitable<NameTy>  {
    private:
        std::string m_name;

    public:
        NameTy(parser::Token& token, const std::string& name):
            Ty(token),
            m_name(name) {}

        NameTy(const NameTy&) = delete;

        NameTy& operator=(const NameTy&) = delete;

        virtual ~NameTy() = default;

        std::string getName() const {
            return m_name;
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
