/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/parser/token.hpp>
#include <hyperscale/syntax/token_kinds.hpp>

namespace hyperscale {
namespace parser {

    std::ostream& operator<<(std::ostream& os, const Token& token) {
        return os << "Token {"
            << "kind: " << static_cast<std::underlying_type<syntax::TokenKind>::type>(token.getKind()) << ", "
            << "value: " << token.getText().str()
            /*<< "start: " << token.start << ", "
            << "end: " << token.end << ", "
            << "length: " << token.length */
            << "}" << std::endl;
    }

} // end of parser namespace
} // end of hyperscale namespace


