/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/identifier.hpp>
#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/parser/token.hpp>
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

namespace hyperscale {
namespace parser {

    /// CLO8 - Return the number of leading ones in the specified 8-bit value.
    static unsigned CLO8(unsigned char C) {
        return llvm::countLeadingOnes(uint32_t(C) << 24);
    }

    /// isStartOfUTF8Character - Return true if this isn't a UTF8 continuation
    /// character, which will be of the form 0b10XXXXXX
    static bool isStartOfUTF8Character(unsigned char C) {
        // RFC 2279: The octet values FE and FF never appear.
        // RFC 3629: The octet values C0, C1, F5 to FF never appear.
        return C <= 0x80 || (C >= 0xC2 && C < 0xF5);
    }

    /// validateUTF8CharacterAndAdvance - Given a pointer to the starting byte of a
    /// UTF8 character, validate it and advance the lexer past it.  This returns the
    /// encoded character or ~0U if the encoding is invalid.
    uint32_t validateUTF8CharacterAndAdvance(const char *&ptr, const char *end) {
        if (ptr >= end) {
            return ~0U;
        }

        unsigned char cur_byte = *ptr++;
        if (cur_byte < 0x80) {
            return cur_byte;
        }

        // Read the number of high bits set, which indicates the number of bytes in
        // the character.
        unsigned encoded_bytes = CLO8(cur_byte);

        // If this is 0b10XXXXXX, then it is a continuation character.
        if (encoded_bytes == 1 || !isStartOfUTF8Character(cur_byte)) {
            // Skip until we get the start of another character.  This is guaranteed to
            // at least stop at the nul at the end of the buffer.
            while (ptr < end && !isStartOfUTF8Character(*ptr)) {
                ++ptr;
            }


            return ~0U;
        }

        // Drop the high bits indicating the # bytes of the result.
        unsigned char_value = (unsigned char)(cur_byte << encoded_bytes) >> encoded_bytes;

        // Read and validate the continuation bytes.
        for (unsigned i = 1; i != encoded_bytes; ++i) {
            if (ptr >= end) {
                return ~0U;
            }

            cur_byte = *ptr;
            // If the high bit isn't set or the second bit isn't clear, then this is not
            // a continuation byte!
            if (cur_byte < 0x80 || cur_byte >= 0xC0) {
                return ~0U;
            }

            // Accumulate our result.
            char_value <<= 6;
            char_value |= cur_byte & 0x3F;
            ++ptr;
        }

        // UTF-16 surrogate pair values are not valid code points.
        if (char_value >= 0xD800 && char_value <= 0xDFFF) {
            return ~0U;
        }

        // If we got here, we read the appropriate number of accumulated bytes.
        // Verify that the encoding was actually minimal.
        // Number of bits in the value, ignoring leading zeros.
        unsigned num_bits = 32 - llvm::countLeadingZeros(char_value);

        if (num_bits <= 5 + 6) {
            return encoded_bytes == 2 ? char_value : ~0U;
        }

        if (num_bits <= 4 + 6 + 6) {
            return encoded_bytes == 3 ? char_value : ~0U;
        }

        return encoded_bytes == 4 ? char_value : ~0U;
    }

    static bool isValidIdentifierContinuationCodePoint(uint32_t c) {
        if (c < 0x80) {
            return clang::isIdentifierBody(c, /*dollar*/true);
        }

        // N1518: Recommendations for extended identifier characters for C and C++
        // Proposed Annex X.1: Ranges of characters allowed
        return c == 0x00A8 || c == 0x00AA || c == 0x00AD || c == 0x00AF
            || (c >= 0x00B2 && c <= 0x00B5) || (c >= 0x00B7 && c <= 0x00BA)
            || (c >= 0x00BC && c <= 0x00BE) || (c >= 0x00C0 && c <= 0x00D6)
            || (c >= 0x00D8 && c <= 0x00F6) || (c >= 0x00F8 && c <= 0x00FF)

            || (c >= 0x0100 && c <= 0x167F)
            || (c >= 0x1681 && c <= 0x180D)
            || (c >= 0x180F && c <= 0x1FFF)

            || (c >= 0x200B && c <= 0x200D)
            || (c >= 0x202A && c <= 0x202E)
            || (c >= 0x203F && c <= 0x2040)
            || c == 0x2054
            || (c >= 0x2060 && c <= 0x206F)

            || (c >= 0x2070 && c <= 0x218F)
            || (c >= 0x2460 && c <= 0x24FF)
            || (c >= 0x2776 && c <= 0x2793)
            || (c >= 0x2C00 && c <= 0x2DFF)
            || (c >= 0x2E80 && c <= 0x2FFF)

            || (c >= 0x3004 && c <= 0x3007)
            || (c >= 0x3021 && c <= 0x302F)
            || (c >= 0x3031 && c <= 0x303F)

            || (c >= 0x3040 && c <= 0xD7FF)

            || (c >= 0xF900 && c <= 0xFD3D)
            || (c >= 0xFD40 && c <= 0xFDCF)
            || (c >= 0xFDF0 && c <= 0xFE44)
            || (c >= 0xFE47 && c <= 0xFFF8)

            || (c >= 0x10000 && c <= 0x1FFFD)
            || (c >= 0x20000 && c <= 0x2FFFD)
            || (c >= 0x30000 && c <= 0x3FFFD)
            || (c >= 0x40000 && c <= 0x4FFFD)
            || (c >= 0x50000 && c <= 0x5FFFD)
            || (c >= 0x60000 && c <= 0x6FFFD)
            || (c >= 0x70000 && c <= 0x7FFFD)
            || (c >= 0x80000 && c <= 0x8FFFD)
            || (c >= 0x90000 && c <= 0x9FFFD)
            || (c >= 0xA0000 && c <= 0xAFFFD)
            || (c >= 0xB0000 && c <= 0xBFFFD)
            || (c >= 0xC0000 && c <= 0xCFFFD)
            || (c >= 0xD0000 && c <= 0xDFFFD)
            || (c >= 0xE0000 && c <= 0xEFFFD);
    }


    static bool isValidIdentifierStartCodePoint(uint32_t c) {
        if (!isValidIdentifierContinuationCodePoint(c)) {
            return false;
        }

        if (c < 0x80 && (isDigit(c) || c == '$')) {
            return false;
        }

        // N1518: Recommendations for extended identifier characters for C and C++
        // Proposed Annex X.2: Ranges of characters disallowed initially
        if ((c >= 0x0300 && c <= 0x036F) ||
            (c >= 0x1DC0 && c <= 0x1DFF) ||
            (c >= 0x20D0 && c <= 0x20FF) ||
            (c >= 0xFE20 && c <= 0xFE2F)
        ) {
            return false;
        }

        return true;
    }

    static bool advanceIf(char const *&ptr, char const *end, bool (*predicate)(uint32_t)) {
        char const *next = ptr;
        uint32_t c = parser::validateUTF8CharacterAndAdvance(next, end);

        if (c == ~0U) {
            return false;
        }

        if (predicate(c)) {
            ptr = next;

            return true;
        }

        return false;
    }

    static bool advanceIfValidStartOfIdentifier(char const *&ptr, char const *end) {
        return advanceIf(ptr, end, isValidIdentifierStartCodePoint);
    }

    static bool advanceIfValidContinuationOfIdentifier(char const *&ptr, char const *end) {
        return advanceIf(ptr, end, isValidIdentifierContinuationCodePoint);
    }

    static bool advanceIfValidStartOfOperator(char const *&ptr, char const *end) {
        return advanceIf(ptr, end, ast::Identifier::isOperatorStartCodePoint);
    }

    static bool advanceIfValidContinuationOfOperator(char const *&ptr, char const *end) {
        return advanceIf(ptr, end, ast::Identifier::isOperatorContinuationCodePoint);
    }

    Lexer::Lexer(const std::string &buffer): m_buffer(buffer) {
        m_buffer_start = m_buffer.data();
        m_buffer_end = m_buffer.data() + m_buffer.size();

        m_cur_ptr = m_buffer_start;

        m_in_sil_mode = false;

        // Check for Unicode BOM at start of file (Only UTF-8 BOM supported now).
        std::size_t BOMLength = llvm::StringSwitch<size_t>(buffer)
            .StartsWith("\xEF\xBB\xBF", 3)
            .Default(0);

        // Since the UTF-8 BOM doesn't carry information (UTF-8 has no dependency
        // on byte order), throw it away.
        m_cur_ptr = m_buffer_start + BOMLength;


        const char *ptr = m_buffer_start + m_buffer.size()-1;

        if (ptr >= m_buffer_start && ptr <= m_buffer_end) {
            m_code_completion_ptr = ptr;
        }
    }

    Token Lexer::lex() {
        auto result = m_next_token;

        if (result.isNot(syntax::TokenKind::Eof)) {
            std::cout << "Debug: result.isNot(syntax::TokenKind::Eof)" << std::endl;

            lexImpl();
        }

        return result;
    }

    void Lexer::formToken(const syntax::TokenKind kind, const char *token_start, bool multiline_string) {
        assert(m_cur_ptr >= m_buffer_start && m_cur_ptr <= m_buffer_end && "Current pointer out of range!");

        llvm::StringRef token_text { token_start, static_cast<size_t>(m_cur_ptr - token_start) };

        m_next_token.setToken(kind, token_text, 0, multiline_string);
    }

    /// lexOperatorIdentifier - Match identifiers formed out of punctuation.
    void Lexer::lexOperatorIdentifier() {

    }

    /// lexIdentifier - Match [a-zA-Z_][a-zA-Z_$0-9]*
    void Lexer::lexIdentifier() {
        const char *token_start = m_cur_ptr - 1;
        m_cur_ptr = token_start;

        bool didStart = advanceIfValidStartOfIdentifier(m_cur_ptr, m_buffer_end);
        assert(didStart && "Unexpected start");
        (void) didStart;

        // Lex [a-zA-Z_$0-9[[:XID_Continue:]]]*
        while (advanceIfValidContinuationOfIdentifier(m_cur_ptr, m_buffer_end));

        syntax::TokenKind kind = kindOfIdentifier(llvm::StringRef(token_start, m_cur_ptr - token_start), m_in_sil_mode);

        return formToken(kind, token_start);
    }

    syntax::TokenKind Lexer::kindOfIdentifier(llvm::StringRef str, bool in_sil_mode) {
        syntax::TokenKind kind = llvm::StringSwitch<syntax::TokenKind>(str)
        #define KEYWORD(kw) \
            .Case(#kw, syntax::TokenKind::Keyword##kw)
        #include "hyperscale/syntax/token_kinds.def"
            .Default(syntax::TokenKind::Identifier);

        // SIL keywords are only active in SIL mode.
        switch (kind) {
        #define SIL_KEYWORD(kw) \
            case syntax::TokenKind::Keyword##kw:
        #include "hyperscale/syntax/token_kinds.def"
                if (!in_sil_mode) {
                    kind = syntax::TokenKind::Identifier;
                }
                break;
            default:
                break;
        }

        return kind;
    }

    bool Lexer::isIdentifier(llvm::StringRef string) {
        if (string.empty()) {
            return false;
        }

        char const *p = string.data(), *end = string.end();
        if (!advanceIfValidStartOfIdentifier(p, end)) {
            return false;
        }

        while (p < end && advanceIfValidContinuationOfIdentifier(p, end));

        return p == end;
    }

    /// \brief Determines if the given string is a valid operator identifier,
    /// without escaping characters.
    bool Lexer::isOperator(llvm::StringRef string) {
        if (string.empty()) {
            return false;
        }

        char const *p = string.data(), *end = string.end();

        if (!advanceIfValidStartOfOperator(p, end)) {
            return false;
        }

        while (p < end && advanceIfValidContinuationOfOperator(p, end));

        return p == end;
    }

    /// lexNumber:
    ///   integer_literal  ::= [0-9][0-9_]*
    ///   integer_literal  ::= 0x[0-9a-fA-F][0-9a-fA-F_]*
    ///   integer_literal  ::= 0o[0-7][0-7_]*
    ///   integer_literal  ::= 0b[01][01_]*
    ///   floating_literal ::= [0-9][0-9]_*\.[0-9][0-9_]*
    ///   floating_literal ::= [0-9][0-9]*\.[0-9][0-9_]*[eE][+-]?[0-9][0-9_]*
    ///   floating_literal ::= [0-9][0-9_]*[eE][+-]?[0-9][0-9_]*
    ///   floating_literal ::= 0x[0-9A-Fa-f][0-9A-Fa-f_]*
    ///                          (\.[0-9A-Fa-f][0-9A-Fa-f_]*)?[pP][+-]?[0-9][0-9_]*
    void Lexer::lexNumber() {

    }

    void Lexer::lexHexNumber() {

    }

    /// lexStringLiteral:
    ///   string_literal ::= ["]([^"\\\n\r]|character_escape)*["]
    ///   string_literal ::= ["]["]["].*["]["]["] - approximately
    void Lexer::lexStringLiteral() {

    }

    void Lexer::lexEscapedIdentifier() {

    }

    void Lexer::lexImpl() {
        assert(m_cur_ptr >= m_buffer_start && m_cur_ptr <= m_buffer_end && "Current pointer out of range!");

        m_next_token.setAtStartOfLine(m_cur_ptr == m_buffer_start);

    Restart:

        // Remember the start of the token so we can form the text range.
        const char *token_start = m_cur_ptr;

        switch ((signed char)*m_cur_ptr++) {
            default: {
            }

            case '\n':
            case '\r':
                // NextToken.setAtStartOfLine(true);
                goto Restart;  // Skip whitespace.

            case ' ':
            case '\t':
            case '\f':
            case '\v':
                goto Restart;  // Skip whitespace.

            case '@': return formToken(syntax::TokenKind::AtSign, token_start);
            case '{': return formToken(syntax::TokenKind::OpenBrace, token_start);
            case '[': return formToken(syntax::TokenKind::OpenBracket, token_start);
            case '(': return formToken(syntax::TokenKind::OpenParen, token_start);
            case '}': return formToken(syntax::TokenKind::CloseBrace, token_start);
            case ']': return formToken(syntax::TokenKind::CloseBracket, token_start);
            case ')': return formToken(syntax::TokenKind::CloseParen, token_start);
            case ',': return formToken(syntax::TokenKind::Comma, token_start);
            case ';': return formToken(syntax::TokenKind::Semi, token_start);
            case ':': return formToken(syntax::TokenKind::Colon, token_start);
            case '\\': return formToken(syntax::TokenKind::Backslash, token_start);

            case '=': case '-': case '+': case '*':
            case '&': case '|':  case '^': case '~': case '.':
                return lexOperatorIdentifier();

            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
            case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
            case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
            case 'V': case 'W': case 'X': case 'Y': case 'Z':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
            case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
            case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
            case 'v': case 'w': case 'x': case 'y': case 'z':
            case '_':
                std::cout << "Debug: lexIdentifier" << std::endl;
                return lexIdentifier();

            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                return lexNumber();

            case '"':
            case '\'':
                return lexStringLiteral();

            case '`':
                return lexEscapedIdentifier();
        }
    }

} // end of parser namespace
} // end of hyperscale namespace
