/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/int-expr.hpp>

namespace hyperscale {
namespace ast {

    inline int IntExpr::getValue() const {
        return m_value;
    }

} // end of ast namespace
} // end of hyperscale namespace
