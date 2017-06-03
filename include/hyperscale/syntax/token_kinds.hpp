/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <ostream>

namespace hyperscale {
namespace syntax {

    enum class TokenKind {
        Unknown = 0,
        Eof,
        CodeComplete,
        Identifier,

        OperBinaryUnspaced,   // "x+y"
        OperBinarySpaced,     // "x + y"
        OperPostfix,
        OperPrefix,
        Dollarident,
        IntegerLiteral,
        FloatingLiteral,
        StringLiteral,
        SilLocalName,      // %42 in SIL mode.
        Comment,

        #define KEYWORD(X) Keyword ## X,
        #define PUNCTUATOR(X, Y) X,
        #define POUND_KEYWORD(X) Pound ## X,
        #include "hyperscale/syntax/token_kinds.def"

        NUM_TOKENS
    };

    std::ostream& operator<<(std::ostream& os, const TokenKind kind);

} // end of syntax namespace
} // end of hyperscale namespace
