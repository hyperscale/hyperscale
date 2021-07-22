/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <sstream>
#include <exception>
#include "lib/hyperscale/parser/token.hpp"

namespace hyperscale {
namespace syntax {

    class InvalidTokenException: public std::exception {
    private:
        parser::Token m_token;
        std::string m_msg;

    public:
        explicit InvalidTokenException(const parser::Token& token);

        virtual ~InvalidTokenException() noexcept;

        virtual const char* what() const noexcept;
    };

} // end of syntax namespace
} // end of hyperscale namespace



