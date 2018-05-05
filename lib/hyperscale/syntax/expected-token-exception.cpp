/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/syntax/expected-token-exception.hpp>

namespace hyperscale {
namespace syntax {

    ExpectedTokenException::ExpectedTokenException(const parser::Token& token, const TokenKind expected)
        : m_token(token),
          m_expected(expected) {
        std::stringstream msg;

        msg << "expected token '" << expected << "'";

        m_msg = msg.str();
    }

    ExpectedTokenException::~ExpectedTokenException() noexcept {
    }

    const char* ExpectedTokenException::what() const noexcept {
        return m_msg.c_str();
    }

} // end of syntax namespace
} // end of hyperscale namespace
