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
#include <hyperscale/parser/token.hpp>

namespace hyperscale {
namespace syntax {

    class ExpectedTokenException: public std::exception {
    private:
        parser::Token m_token;
        TokenKind m_expected;
        std::string m_msg;

    public:
        explicit ExpectedTokenException(const parser::Token& token, const TokenKind expected);

        virtual ~ExpectedTokenException() noexcept;

        virtual const char* what() const noexcept;
    };

} // end of syntax namespace
} // end of hyperscale namespace
