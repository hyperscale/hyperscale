

/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/op-expr.hpp>

namespace hyperscale {
namespace ast {

    std::ostream& operator<<(std::ostream& os, Operator op) {
        os << OperatorNames[static_cast<int>(op)];

        return os;
    }

} // end of ast namespace
} // end of hyperscale namespace



