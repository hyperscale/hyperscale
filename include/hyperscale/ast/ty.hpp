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

    class Ty: public Node {
    public:
        Ty(parser::Token& token): Node(token) {}

        Ty(const Ty&) = delete;

        Ty& operator=(const Ty&) = delete;

        virtual ~Ty() = default;
    };

} // end of ast namespace
} // end of hyperscale namespace
