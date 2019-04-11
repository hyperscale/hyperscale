/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

// #include <hyperscale/ast/identifier.hpp>
#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/parser/token.hpp>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/Support/MathExtras.h>
#include <string>
// FIXME: Figure out if this can be migrated to LLVM.
#include <clang/Basic/CharInfo.h>
#include <iostream>

// clang::isIdentifierHead and clang::isIdentifierBody are deliberately not in
// this list as a reminder that they are using C rules for identifiers.
// (Admittedly these are the same as Swift's right now.)
using clang::isAlphanumeric;
using clang::isDigit;
using clang::isHexDigit;
using clang::isHorizontalWhitespace;
using clang::isPrintable;
using clang::isWhitespace;

#define WHITESPACE \
         ' ': \
    case '\t'

#define DIGIT_NON_ZERO \
         '1': \
    case '2': \
    case '3': \
    case '4': \
    case '5': \
    case '6': \
    case '7': \
    case '8': \
    case '9'
#define DIGIT \
         '0': \
    case DIGIT_NON_ZERO

#define ALPHA \
         'a': \
    case 'b': \
    case 'c': \
    case 'd': \
    case 'e': \
    case 'f': \
    case 'g': \
    case 'h': \
    case 'i': \
    case 'j': \
    case 'k': \
    case 'l': \
    case 'm': \
    case 'n': \
    case 'o': \
    case 'p': \
    case 'q': \
    case 'r': \
    case 's': \
    case 't': \
    case 'u': \
    case 'v': \
    case 'w': \
    case 'x': \
    case 'y': \
    case 'z': \
    case 'A': \
    case 'B': \
    case 'C': \
    case 'D': \
    case 'E': \
    case 'F': \
    case 'G': \
    case 'H': \
    case 'I': \
    case 'J': \
    case 'K': \
    case 'L': \
    case 'M': \
    case 'N': \
    case 'O': \
    case 'P': \
    case 'Q': \
    case 'R': \
    case 'S': \
    case 'T': \
    case 'U': \
    case 'V': \
    case 'W': \
    case 'X': \
    case 'Y': \
    case 'Z'

#define SYMBOL_CHAR \
    ALPHA: \
    case DIGIT: \
    case '_'

namespace hyperscale {
namespace parser {

    struct Keyword {
        std::string name;
        syntax::TokenKind kind;
    };

    std::vector<Keyword> keywords = {
        {"var", syntax::TokenKind::KeywordVar},
        {"func", syntax::TokenKind::KeywordFunc},
        {"return", syntax::TokenKind::KeywordReturn}
    };
/*
    static bool is_exponent_signifier(const char c, int radix) {
        if (radix == 16) {
            return c == 'p' || c == 'P';
        } else {
            return c == 'e' || c == 'E';
        }
    }
*/
    Lexer::Lexer(const std::string &buffer):
        m_buffer(buffer),
        m_current_line(1),
        m_current_col(0),
        m_state(LexerStateStart),
        m_token_text("") {
        // Check for Unicode BOM at start of file (Only UTF-8 BOM supported now).
        std::size_t BOMLength = llvm::StringSwitch<size_t>(buffer)
            .StartsWith("\xEF\xBB\xBF", 3)
            .Default(0);

        m_pos = BOMLength;
    }

    void Lexer::beginToken(syntax::TokenKind kind) {
        m_current_token.setStartOffset(m_pos);
        m_current_token.setLine(m_current_line);
        m_current_token.setColumn(m_current_col);
        m_current_token.setKind(kind);
    }

    void Lexer::appendCharToken(const char c) {
        m_token_text += c;
    }

    void Lexer::endToken() {
        m_current_token.setEndOffset(m_pos);
        m_current_token.setText(llvm::StringRef(m_token_text));

        if (m_current_token.is(syntax::TokenKind::Identifier)) {
            for (auto const& keyword: keywords) {
                if (m_current_token.getText().compare(keyword.name) == 0) {
                    m_current_token.setKind(keyword.kind);
                }
            }
        }
    }

    void Lexer::resetToken() {
        m_token_text.clear();
        m_current_token = Token();
    }

    // var i = 1 + 2;
    Token Lexer::lex() {
        resetToken();

        for (; m_pos < m_buffer.size(); m_pos += 1) {
            const char c = m_buffer[m_pos];

            m_current_col++;

            switch (m_state) {
                case LexerStateError:
                    break;
                case LexerStateStart:
                    switch (c) {
                        case '\n':
                            m_current_line++;
                            m_current_col = 0;
                            break;
                        case WHITESPACE:
                            break;
                        case ALPHA:
                        case '_':
                            m_state = LexerStateSymbol;
                            beginToken(syntax::TokenKind::Identifier);
                            appendCharToken(c);

                            break;
                        case DIGIT:
                            m_state = LexerStateNumber;
                            beginToken(syntax::TokenKind::IntegerLiteral);
                            appendCharToken(c);
                            m_radix = 10;

                            break;
                        case '+':
                            beginToken(syntax::TokenKind::KeywordOperator);
                            appendCharToken(c);
                            m_pos += 1;
                            endToken();

                            return m_current_token;
                        case '=':
                            m_state = LexerStateStart;
                            beginToken(syntax::TokenKind::Equal);
                            appendCharToken(c);
                            m_pos += 1;
                            endToken();

                            return m_current_token;
                        case ';':
                            m_state = LexerStateStart;
                            beginToken(syntax::TokenKind::Semi);
                            appendCharToken(c);
                            m_pos += 1;
                            endToken();

                            return m_current_token;
                        case ':':
                            m_state = LexerStateStart;
                            beginToken(syntax::TokenKind::Colon);
                            appendCharToken(c);
                            m_pos += 1;
                            endToken();

                            return m_current_token;
                        case '(':
                            m_state = LexerStateStart;
                            beginToken(syntax::TokenKind::OpenParen);
                            appendCharToken(c);
                            m_pos += 1;
                            endToken();

                            return m_current_token;
                        case ')':
                            m_state = LexerStateStart;
                            beginToken(syntax::TokenKind::CloseParen);
                            appendCharToken(c);
                            m_pos += 1;
                            endToken();

                            return m_current_token;
                        case '{':
                            m_state = LexerStateStart;
                            beginToken(syntax::TokenKind::OpenBrace);
                            appendCharToken(c);
                            m_pos += 1;
                            endToken();

                            return m_current_token;
                        case '}':
                            m_state = LexerStateStart;
                            beginToken(syntax::TokenKind::CloseBrace);
                            appendCharToken(c);
                            m_pos += 1;
                            endToken();

                            return m_current_token;
                        default:
                            // std::cout << "Default Start State" << std::endl;

                            continue;
                    }
                    break;
                case LexerStateSymbol:
                    switch (c) {
                        case SYMBOL_CHAR:
                            appendCharToken(c);
                            break;
                        default:
                            // m_pos -= 1;
                            endToken();
                            m_state = LexerStateStart;
                            m_current_col -= 1;

                            return m_current_token;
                    }
                    break;
                case LexerStateNumber:
                    switch (c) {
                        case DIGIT:
                            appendCharToken(c);
                            break;
                        default:
                            // m_pos -= 1;
                            endToken();
                            m_state = LexerStateStart;
                            m_current_col -= 1;

                            return m_current_token;
                    }
                    break;

                default:
                    break;

            }
        }

        beginToken(syntax::TokenKind::Eof);
        endToken();

        return m_current_token;
    }

} // end of parser namespace
} // end of hyperscale namespace
