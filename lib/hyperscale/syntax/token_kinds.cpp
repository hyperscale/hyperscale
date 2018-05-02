/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <type_traits>
#include <hyperscale/syntax/token_kinds.hpp>

namespace hyperscale {
namespace syntax {

    std::ostream& operator<<(std::ostream& os, const TokenKind kind) {
        return os << TokenNames.at(kind);
    }

} // end of syntax namespace
} // end of hyperscale namespace
