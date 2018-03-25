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

    class Decl: public Node
    {
    protected:
        std::string m_name;

    public:
        Decl(parser::Token& token, std::string name):
            Node(token),
            m_name(name) {}

        Decl(const Decl&) = delete;

        Decl& operator=(const Decl&) = delete;

        virtual ~Decl() = default;

        std::string getName() const {
            return m_name;
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
