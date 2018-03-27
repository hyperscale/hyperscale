/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

namespace hyperscale {
namespace ast {

    template <class E>
    void Visitor::operator()(E* e) {
        e->accept(*this);
    }

    template <typename E>
    void Visitor::accept(E* e) {
        if (e) {
            e->accept(*this);
        }
    }

} // end of ast namespace
} // end of hyperscale namespace


