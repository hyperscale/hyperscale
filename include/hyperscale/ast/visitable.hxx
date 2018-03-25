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
#include <hyperscale/ast/visitable.hpp>

namespace hyperscale {
namespace ast {

    template<class Derived>
    void Visitable<Derived>::accept(Visitor& visitor) {
        visitor(static_cast<Derived&>(*this));
    }

} // end of ast namespace
} // end of hyperscale namespace


