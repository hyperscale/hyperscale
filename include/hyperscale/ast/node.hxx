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

    inline std::size_t Node::getLine() const {
        return m_line;
    }

    inline std::size_t Node::getColumn() const {
        return m_column;
    }

} // end of ast namespace
} // end of hyperscale namespace
