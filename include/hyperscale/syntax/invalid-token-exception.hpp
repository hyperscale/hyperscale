/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <exception>
#include <hyperscale/parser/token.hpp>

namespace hyperscale {
namespace syntax {

    class InvalidTokenException: public std::exception {
    private:
        parser::Token m_token;

    public:
        InvalidTokenException(parser::Token token);

        const char* what() const throw();
    };

} // end of syntax namespace
} // end of hyperscale namespace



