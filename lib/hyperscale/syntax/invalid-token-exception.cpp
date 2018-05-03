/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <sstream>
#include <hyperscale/syntax/invalid-token-exception.hpp>

namespace hyperscale {
namespace syntax {

    InvalidTokenException::InvalidTokenException(parser::Token token)
        : m_token(token) {}

    const char* InvalidTokenException::what() const throw() {
        std::stringstream ss("invalid token ");

        ss << "'" << m_token << "'";

        return ss.str().c_str();
    }

} // end of syntax namespace
} // end of hyperscale namespace
