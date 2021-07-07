/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include "lib/hyperscale/parser/token.hpp"
#include "lib/hyperscale/syntax/token-kinds.hpp"
#include <istream>
#include <string>
#include <vector>

namespace hyperscale {
namespace parser {

    enum LexerState {
        LexerStateStart,
        LexerStateSymbol,
        LexerStateSymbolFirstC,
        LexerStateZero, // "0", which might lead to "0x"
        LexerStateNumber, // "123", "0x123"
        LexerStateNumberDot,
        LexerStateFloatFraction, // "123.456", "0x123.456"
        LexerStateFloatExponentUnsigned, // "123.456e", "123e", "0x123p"
        LexerStateFloatExponentNumber, // "123.456e-", "123.456e5", "123.456e5e-5"
        LexerStateString,
        LexerStateStringEscape,
        LexerStateCharLiteral,
        LexerStateCharLiteralEnd,
        LexerStateSawStar,
        LexerStateSawStarPercent,
        LexerStateSawSlash,
        LexerStateSawBackslash,
        LexerStateSawPercent,
        LexerStateSawPlus,
        LexerStateSawPlusPercent,
        LexerStateSawDash,
        LexerStateSawMinusPercent,
        LexerStateSawAmpersand,
        LexerStateSawCaret,
        LexerStateSawBar,
        LexerStateSawBarBar,
        LexerStateLineComment,
        LexerStateLineString,
        LexerStateLineStringEnd,
        LexerStateLineStringContinue,
        LexerStateLineStringContinueC,
        LexerStateSawEq,
        LexerStateSawBang,
        LexerStateSawLessThan,
        LexerStateSawLessThanLessThan,
        LexerStateSawGreaterThan,
        LexerStateSawGreaterThanGreaterThan,
        LexerStateSawDot,
        LexerStateSawDotDot,
        LexerStateSawQuestionMark,
        LexerStateSawAtSign,
        LexerStateCharCode,
        LexerStateError,
    };


    class Lexer {
    private:
        const std::string& m_buffer;

        std::size_t m_pos;

        Token m_current_token;

        std::size_t m_current_line;
        std::size_t m_current_col;

        LexerState m_state;

        std::string m_token_text;

        int m_radix;

        void beginToken(syntax::TokenKind kind);

        void appendCharToken(const char c);

        void endToken();

        void resetToken();

    public:
        Lexer(const std::string &buffer);

        /// \brief Returns true if this lexer will produce a code completion token.
        bool isEndOfFile() const {
            return m_pos == m_buffer.size();
        }

        Token lex();
    };

} // end of parser namespace
} // end of hyperscale namespace
