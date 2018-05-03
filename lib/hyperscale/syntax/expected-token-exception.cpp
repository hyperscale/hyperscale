/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <sstream>
#include <hyperscale/syntax/expected-token-exception.hpp>

namespace hyperscale {
namespace syntax {

    ExpectedTokenException::ExpectedTokenException(parser::Token token, TokenKind expected)
        : m_token(token),
          m_expected(expected) {}

    const char* ExpectedTokenException::what() const throw() {
        std::stringstream ss("expected token ");

        ss << "'" << m_expected << "'";

        return ss.str().c_str();
    }

} // end of syntax namespace
} // end of hyperscale namespace
