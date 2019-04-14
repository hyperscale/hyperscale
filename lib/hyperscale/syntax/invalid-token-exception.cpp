/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/syntax/invalid-token-exception.hpp>

namespace hyperscale {
namespace syntax {

    InvalidTokenException::InvalidTokenException(const parser::Token& token)
        : m_token(token) {
        std::stringstream msg;

        msg << "invalid token '" << token.getKind() << "'";

        m_msg = msg.str();
    }

    InvalidTokenException::~InvalidTokenException() noexcept {}

    const char* InvalidTokenException::what() const noexcept {
        return m_msg.c_str();
    }

} // end of syntax namespace
} // end of hyperscale namespace
