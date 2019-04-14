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
#include <map>
#include <string>

namespace hyperscale {
namespace syntax {

    enum class TokenKind {
        Unknown = 0,
        Eof,
        Identifier,

        OperBinaryUnspaced,   // "x+y"
        OperBinarySpaced,     // "x + y"
        OperPostfix,
        OperPrefix,
        DollarIdent,
        IntegerLiteral,
        FloatingLiteral,
        StringLiteral,
        Comment,

        #define KEYWORD(X) Keyword ## X,
        #define PUNCTUATOR(X, Y) X,
        #define POUND_KEYWORD(X) Pound ## X,
        #include <hyperscale/syntax/token-kinds.def>
        #undef KEYWORD
        #undef PUNCTUATOR
        #undef POUND_KEYWORD

        NUM_TOKENS
    };

    static const std::map<TokenKind, std::string> TokenNames = {
        {TokenKind::Unknown, "Unknown"},
        {TokenKind::Eof, "Eof"},
        {TokenKind::Identifier, "Identifier"},
        {TokenKind::OperBinaryUnspaced, "OperBinaryUnspaced"},
        {TokenKind::OperBinarySpaced, "OperBinarySpaced"},
        {TokenKind::OperPostfix, "OperPostfix"},
        {TokenKind::OperPrefix, "OperPrefix"},
        {TokenKind::DollarIdent, "DollarIdent"},
        {TokenKind::IntegerLiteral, "IntegerLiteral"},
        {TokenKind::FloatingLiteral, "FloatingLiteral"},
        {TokenKind::StringLiteral, "StringLiteral"},
        {TokenKind::Comment, "Comment"},

        #define KEYWORD(X) {TokenKind::Keyword ## X, "Keyword" # X},
        #define PUNCTUATOR(X, Y) {TokenKind::X, # X},
        #define POUND_KEYWORD(X) {TokenKind::Pound ## X, "Keyword" # X},
        #include <hyperscale/syntax/token-kinds.def>
        #undef KEYWORD
        #undef PUNCTUATOR
        #undef POUND_KEYWORD
    };

    std::ostream& operator<<(std::ostream& os, const TokenKind kind);

} // end of syntax namespace
} // end of hyperscale namespace
