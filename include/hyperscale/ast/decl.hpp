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
    public:
        Decl();

        Decl(parser::Token& token);

        Decl(const Decl&) = delete;

        Decl& operator=(const Decl&) = delete;

        virtual ~Decl() = default;
    };

} // end of ast namespace
} // end of hyperscale namespace
