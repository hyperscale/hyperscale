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

    /// Is the operator beginning at the given character "left-bound"?
    static bool isLeftBound(const char *tok_begin, const char *buffer_begin) {
        // The first character in the file is not left-bound.
        if (tok_begin == buffer_begin) {
            return false;
        }

        switch (tok_begin[-1]) {
        case ' ': case '\r': case '\n': case '\t': // whitespace
        case '(': case '[': case '{':              // opening delimiters
        case ',': case ';': case ':':              // expression separators
        case '\0':                                 // whitespace / last char in file
            return false;

        case '/':
            if (tok_begin - 1 != buffer_begin && tok_begin[-2] == '*') {
                return false; // End of a slash-star comment, so whitespace.
            } else {
                return true;
            }

        default:
            return true;
        }
    }


    /// Is the operator ending at the given character (actually one past the end)
    /// "right-bound"?
    ///
    /// The code-completion point is considered right-bound.
    static bool isRightBound(const char *token_end, bool is_left_bound, const char *code_completion_ptr) {
        switch (*token_end) {
        case ' ': case '\r': case '\n': case '\t': // whitespace
        case ')': case ']': case '}':              // closing delimiters
        case ',': case ';': case ':':              // expression separators
            return false;

        case '\0':
            if (token_end == code_completion_ptr) {         // code-completion
                return true;
            }

            return false;                            // whitespace / last char in file

        case '.':
            // Prefer the '^' in "x^.y" to be a postfix op, not binary, but the '^' in
            // "^.y" to be a prefix op, not binary.
            return !is_left_bound;

        case '/':
            // A following comment counts as whitespace, so this token is not right bound.
            if (token_end[1] == '/' || token_end[1] == '*') {
                return false;
            } else {
                return true;
            }

        default:
            return true;
        }
    }

    static bool rangeContainsPlaceholderEnd(const char *cur_ptr, const char *end) {
        for (auto sub_str = cur_ptr; sub_str != end - 1; ++sub_str) {
            if (sub_str[0] == '\n') {
                return false;
            }

            if (sub_str[0] == '#' && sub_str[1] == '>') {
                return true;
            }
        }

        return false;
    }

    /// Find the end of a version control conflict marker.
    static const char *findConflictEnd(const char *cur_ptr, const char *buffer_end, ConflictMarkerKind CMK) {
        llvm::StringRef terminator = CMK == ConflictMarkerKind::Perforce ? "<<<<\n" : ">>>>>>> ";
        std::size_t term_len = terminator.size();

        // Get a reference to the rest of the buffer minus the length of the start
        // of the conflict marker.
        auto rest_of_buffer = llvm::StringRef(cur_ptr, buffer_end - cur_ptr).substr(term_len);
        std::size_t end_pos = rest_of_buffer.find(terminator);

        while (end_pos != llvm::StringRef::npos) {
            // Must occur at start of line.
            if (end_pos != 0 && (rest_of_buffer[end_pos - 1] == '\r' || rest_of_buffer[end_pos - 1] == '\n')) {
                return rest_of_buffer.data() + end_pos;
            }

            rest_of_buffer = rest_of_buffer.substr(end_pos + term_len);
            end_pos = rest_of_buffer.find(terminator);
        }

        return nullptr;
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
            // std::cout << "Debug: result.isNot(syntax::TokenKind::Eof)" << std::endl;

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
        const char *token_start = m_cur_ptr-1;

        m_cur_ptr = token_start;

        bool did_start = advanceIfValidStartOfOperator(m_cur_ptr, m_buffer_end);
        assert(did_start && "unexpected operator start");
        (void) did_start;

        do {
            if (m_cur_ptr != m_buffer_end && m_in_sil_body && (*m_cur_ptr == '!' || *m_cur_ptr == '?')) {
                // When parsing SIL body, '!' and '?' are special token and can't be
                // in the middle of an operator.
                break;
            }

            // '.' cannot appear in the middle of an operator unless the operator
            // started with a '.'.
            if (*m_cur_ptr == '.' && *token_start != '.') {
                break;
            }

            if (
                ast::Identifier::isEditorPlaceholder(llvm::StringRef(m_cur_ptr, m_buffer_end - m_cur_ptr)) &&
                rangeContainsPlaceholderEnd(m_cur_ptr + 2, m_buffer_end)
            ) {
                break;
            }
        } while (advanceIfValidContinuationOfOperator(m_cur_ptr, m_buffer_end));

        if (m_cur_ptr - token_start > 2) {
            // If there is a "//" or "/*" in the middle of an identifier token,
            // it starts a comment.
            for (auto ptr = token_start + 1; ptr != m_cur_ptr - 1; ++ptr) {
                if (ptr[0] == '/' && (ptr[1] == '/' || ptr[1] == '*')) {
                    m_cur_ptr = ptr;
                    break;
                }
            }
        }

        // Decide between the binary, prefix, and postfix cases.
        // It's binary if either both sides are bound or both sides are not bound.
        // Otherwise, it's postfix if left-bound and prefix if right-bound.
        bool left_bound = isLeftBound(token_start, m_buffer_start);
        bool right_bound = isRightBound(m_cur_ptr, left_bound, m_code_completion_ptr);

        // Match various reserved words.
        if (m_cur_ptr - token_start == 1) {
            switch (token_start[0]) {
            case '=':
                //@TODO
                /*if (left_bound != right_bound) {
                    auto d = diagnose(token_start, diag::lex_unary_equal);
                    if (left_bound) {
                        d.fixItInsert(getSourceLoc(token_start), " ");
                    } else {
                        d.fixItInsert(getSourceLoc(token_start+1), " ");
                    }
                }*/

                // always emit 'tok::equal' to avoid trickle down parse errors
                return formToken(syntax::TokenKind::Equal, token_start);

            case '&':
                if (left_bound == right_bound || left_bound) {
                    break;
                }

                return formToken(syntax::TokenKind::AmpPrefix, token_start);

            case '.': {
                if (left_bound == right_bound) {
                    return formToken(syntax::TokenKind::Period, token_start);
                }

                if (right_bound) {
                    return formToken(syntax::TokenKind::PeriodPrefix, token_start);
                }

                // If left bound but not right bound, handle some likely situations.

                // If there is just some horizontal whitespace before the next token, its
                // addition is probably incorrect.
                const char *after_horz_whitespace = m_cur_ptr;

                while (*after_horz_whitespace == ' ' || *after_horz_whitespace == '\t') {
                    ++after_horz_whitespace;
                }

                // First, when we are code completing "x. <ESC>", then make sure to return
                // a tok::period, since that is what the user is wanting to know about.
                if (*after_horz_whitespace == '\0' && after_horz_whitespace == m_code_completion_ptr) {
                    //@TODO
                    //diagnose(token_start, diag::expected_member_name);

                    return formToken(syntax::TokenKind::Period, token_start);
                }

                if (
                    isRightBound(after_horz_whitespace, left_bound, m_code_completion_ptr) &&
                    // Don't consider comments to be this.  A leading slash is probably
                    // either // or /* and most likely occurs just in our testsuite for
                    // expected-error lines.
                    *after_horz_whitespace != '/'
                ) {
                    //@TODO
                    /*diagnose(token_start, diag::extra_whitespace_period)
                        .fixItRemoveChars(
                            getSourceLoc(CurPtr),
                            getSourceLoc(after_horz_whitespace)
                        );*/

                    return formToken(syntax::TokenKind::Period, token_start);
                }

                // Otherwise, it is probably a missing member.
                //@TODO
                //diagnose(token_start, diag::expected_member_name);

                return formToken(syntax::TokenKind::Unknown, token_start);
            }

            case '?':
                if (left_bound) {
                    return formToken(syntax::TokenKind::QuestionPostfix, token_start);
                }

                return formToken(syntax::TokenKind::QuestionInfix, token_start);
            }
        } else if (m_cur_ptr - token_start == 2) {
            switch ((token_start[0] << 8) | token_start[1]) {
            case ('-' << 8) | '>': // ->
                return formToken(syntax::TokenKind::Arrow, token_start);
            case ('*' << 8) | '/': // */
                //@TODO
                //diagnose(token_start, diag::lex_unexpected_block_comment_end);

                return formToken(syntax::TokenKind::Unknown, token_start);
            }
        } else {
            // Verify there is no "*/" in the middle of the identifier token, we reject
            // it as potentially ending a block comment.
            auto pos = llvm::StringRef(token_start, m_cur_ptr - token_start).find("*/");
            if (pos != llvm::StringRef::npos) {
                //@TODO
                //diagnose(token_start + pos, diag::lex_unexpected_block_comment_end);

                return formToken(syntax::TokenKind::Unknown, token_start);
            }
        }

        if (left_bound == right_bound) {
            return formToken(left_bound ? syntax::TokenKind::OperBinaryUnspaced : syntax::TokenKind::OperBinarySpaced, token_start);
        }

        return formToken(left_bound ? syntax::TokenKind::OperPostfix : syntax::TokenKind::OperPrefix, token_start);
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
        const char *token_start = m_cur_ptr-1;

        assert((isDigit(*token_start) || *token_start == '.') && "Unexpected start");

        auto expected_digit = [&](const char *loc/*, Diag<> msg*/) {
            //@TODO
            //diagnose(loc, msg);
            while (advanceIfValidContinuationOfIdentifier(m_cur_ptr, m_buffer_end));

            return formToken(syntax::TokenKind::Unknown, token_start);
        };

        if (*token_start == '0' && *m_cur_ptr == 'x') {
            return lexHexNumber();
        }

        if (*token_start == '0' && *m_cur_ptr == 'o') {
            // 0o[0-7][0-7_]*
            ++m_cur_ptr;
            if (*m_cur_ptr < '0' || *m_cur_ptr > '7') {
                //@TODO
                return expected_digit(m_cur_ptr/*, diag::lex_expected_digit_in_int_literal*/);
            }

            while ((*m_cur_ptr >= '0' && *m_cur_ptr <= '7') || *m_cur_ptr == '_') {
                ++m_cur_ptr;
            }

            return formToken(syntax::TokenKind::IntegerLiteral, token_start);
        }

        if (*token_start == '0' && *m_cur_ptr == 'b') {
            // 0b[01][01_]*
            ++m_cur_ptr;

            if (*m_cur_ptr != '0' && *m_cur_ptr != '1') {
                //@TODO
                return expected_digit(m_cur_ptr/*, diag::lex_expected_digit_in_int_literal*/);
            }

            while (*m_cur_ptr == '0' || *m_cur_ptr == '1' || *m_cur_ptr == '_') {
                ++m_cur_ptr;
            }

            return formToken(syntax::TokenKind::IntegerLiteral, token_start);
        }

        // Handle a leading [0-9]+, lexing an integer or falling through if we have a
        // floating point value.
        while (isDigit(*m_cur_ptr) || *m_cur_ptr == '_') {
            ++m_cur_ptr;
        }

        // Lex things like 4.x as '4' followed by a tok::period.
        if (*m_cur_ptr == '.') {
            // NextToken is the soon to be previous token
            // Therefore: x.0.1 is sub-tuple access, not x.float_literal
            if (!isDigit(m_cur_ptr[1]) || m_next_token.is(syntax::TokenKind::Period)) {
                return formToken(syntax::TokenKind::IntegerLiteral, token_start);
            }
        } else {
            // Floating literals must have '.', 'e', or 'E' after digits.  If it is
            // something else, then this is the end of the token.
            if (*m_cur_ptr != 'e' && *m_cur_ptr != 'E') {
                char const *tmp = m_cur_ptr;

                if (advanceIfValidContinuationOfIdentifier(m_cur_ptr, m_buffer_end)) {
                    //@TODO
                    return expected_digit(tmp/*, diag::lex_expected_digit_in_int_literal*/);
                }

                return formToken(syntax::TokenKind::IntegerLiteral, token_start);
            }
        }

        // Lex decimal point.
        if (*m_cur_ptr == '.') {
            ++m_cur_ptr;

            // Lex any digits after the decimal point.
            while (isDigit(*m_cur_ptr) || *m_cur_ptr == '_') {
                ++m_cur_ptr;
            }
        }

        // Lex exponent.
        if (*m_cur_ptr == 'e' || *m_cur_ptr == 'E') {
            ++m_cur_ptr;  // Eat the 'e'

            if (*m_cur_ptr == '+' || *m_cur_ptr == '-') {
                ++m_cur_ptr;  // Eat the sign.
            }

            if (!isDigit(*m_cur_ptr)) {
                //@TODO
                return expected_digit(m_cur_ptr/*, diag::lex_expected_digit_in_fp_exponent*/);
            }

            while (isDigit(*m_cur_ptr) || *m_cur_ptr == '_') {
                ++m_cur_ptr;
            }
        }

        return formToken(syntax::TokenKind::FloatingLiteral, token_start);
    }

    void Lexer::lexHexNumber() {
        // We assume we're starting from the 'x' in a '0x...' floating-point literal.
        assert(*m_cur_ptr == 'x' && "not a hex literal");

        const char *token_start = m_cur_ptr-1;
        assert(*token_start == '0' && "not a hex literal");

        // 0x[0-9a-fA-F][0-9a-fA-F_]*
        ++m_cur_ptr;
        if (!isHexDigit(*m_cur_ptr)) {
            //@TODO
            //diagnose(m_cur_ptr, diag::lex_expected_digit_in_int_literal);

            while (advanceIfValidContinuationOfIdentifier(m_cur_ptr, m_buffer_end));

            return formToken(syntax::TokenKind::Unknown, token_start);
        }

        while (isHexDigit(*m_cur_ptr) || *m_cur_ptr == '_') {
            ++m_cur_ptr;
        }

        if (*m_cur_ptr != '.' && *m_cur_ptr != 'p' && *m_cur_ptr != 'P') {
            return formToken(syntax::TokenKind::IntegerLiteral, token_start);
        }

        const char *ptr_on_dot = nullptr;

        // (\.[0-9A-Fa-f][0-9A-Fa-f_]*)?
        if (*m_cur_ptr == '.') {
            ptr_on_dot = m_cur_ptr;
            ++m_cur_ptr;

            // If the character after the '.' is not a digit, assume we have an int
            // literal followed by a dot expression.
            if (!isHexDigit(*m_cur_ptr)) {
                --m_cur_ptr;

                return formToken(syntax::TokenKind::IntegerLiteral, token_start);
            }

            while (isHexDigit(*m_cur_ptr) || *m_cur_ptr == '_') {
                ++m_cur_ptr;
            }

            if (*m_cur_ptr != 'p' && *m_cur_ptr != 'P') {
                if (!isDigit(ptr_on_dot[1])) {
                    // e.g: 0xff.description
                    m_cur_ptr = ptr_on_dot;

                    return formToken(syntax::TokenKind::IntegerLiteral, token_start);
                }

                //@TODO
                //diagnose(m_cur_ptr, diag::lex_expected_binary_exponent_in_hex_float_literal);

                return formToken(syntax::TokenKind::Unknown, token_start);
            }
        }

        // [pP][+-]?[0-9][0-9_]*
        assert(*m_cur_ptr == 'p' || *m_cur_ptr == 'P' && "not at a hex float exponent?!");
        ++m_cur_ptr;

        bool signed_exponent = false;
        if (*m_cur_ptr == '+' || *m_cur_ptr == '-') {
            ++m_cur_ptr;  // Eat the sign.
            signed_exponent = true;
        }

        if (!isDigit(*m_cur_ptr)) {
            if (ptr_on_dot && !isDigit(ptr_on_dot[1]) && !signed_exponent) {
                // e.g: 0xff.fpValue, 0xff.fp
                m_cur_ptr = ptr_on_dot;

                return formToken(syntax::TokenKind::IntegerLiteral, token_start);
            }

            // Note: 0xff.fp+otherExpr can be valid expression. But we don't accept it.
            //@TODO
            //diagnose(m_cur_ptr, diag::lex_expected_digit_in_fp_exponent);

            return formToken(syntax::TokenKind::Unknown, token_start);
        }

        while (isDigit(*m_cur_ptr) || *m_cur_ptr == '_') {
            ++m_cur_ptr;
        }

        return formToken(syntax::TokenKind::FloatingLiteral, token_start);
    }

    /// lexStringLiteral:
    ///   string_literal ::= ["]([^"\\\n\r]|character_escape)*["]
    ///   string_literal ::= ["]["]["].*["]["]["] - approximately
    void Lexer::lexStringLiteral() {

    }

    void Lexer::lexEscapedIdentifier() {

    }

    /// lexHash - Handle #], #! for shebangs, and the family of #identifiers.
    void Lexer::lexHash() {
        const char *token_start = m_cur_ptr - 1;

        // Scan for [a-zA-Z]+ to see what we match.
        const char *tmp_ptr = m_cur_ptr;
        if (clang::isIdentifierHead(*tmp_ptr)) {
            do {
                ++tmp_ptr;
            } while (clang::isIdentifierBody(*tmp_ptr));
        }

        // Map the character sequence onto
        syntax::TokenKind kind = llvm::StringSwitch<syntax::TokenKind>(llvm::StringRef(m_cur_ptr, tmp_ptr - m_cur_ptr))
        #define POUND_KEYWORD(id) \
            .Case(#id, syntax::TokenKind::Pound##id)
        #include "hyperscale/syntax/token_kinds.def"
            .Default(syntax::TokenKind::Pound);

        // If we didn't find a match, then just return tok::pound.  This is highly
        // dubious in terms of error recovery, but is useful for code completion and
        // SIL parsing.
        if (kind == syntax::TokenKind::Pound) {
            return formToken(syntax::TokenKind::Pound, token_start);
        }

        // If we found something specific, return it.
        m_cur_ptr = tmp_ptr;

        return formToken(kind, token_start);
    }

    void Lexer::skipSlashSlashComment() {
        assert(m_cur_ptr[-1] == '/' && m_cur_ptr[0] == '/' && "Not a // comment");

        skipToEndOfLine();
    }

    void Lexer::skipToEndOfLine() {
        while (1) {
            switch (*m_cur_ptr++) {
            case '\n':
            case '\r':
                m_next_token.setAtStartOfLine(true);

                return;  // If we found the end of the line, return.
            default:
                // If this is a "high" UTF-8 character, validate it.
                if ((signed char)(m_cur_ptr[-1]) < 0) {
                    --m_cur_ptr;

                    const char *char_start = m_cur_ptr;

                    if (validateUTF8CharacterAndAdvance(m_cur_ptr, m_buffer_end) == ~0U) {
                        //@TODO
                        // diagnose(char_start, diag::lex_invalid_utf8);
                    }
                }
                break;   // Otherwise, eat other characters.
            case 0:
                // If this is a random nul character in the middle of a buffer, skip it as
                // whitespace.
                if (m_cur_ptr-1 != m_buffer_end) {
                    //@TODO
                    // diagnoseEmbeddedNul(Diags, m_cur_ptr-1);
                    break;
                }

                // Otherwise, the last line of the file does not have a newline.
                --m_cur_ptr;

                return;
            }
        }
    }

    /// skipSlashStarComment - /**/ comments are skipped (treated as whitespace).
    /// Note that (unlike in C) block comments can be nested.
    void Lexer::skipSlashStarComment() {
        const char *start_ptr = m_cur_ptr-1;

        assert(m_cur_ptr[-1] == '/' && m_cur_ptr[0] == '*' && "Not a /* comment");
        // Make sure to advance over the * so that we don't incorrectly handle /*/ as
        // the beginning and end of the comment.

        ++m_cur_ptr;

        // /**/ comments can be nested, keep track of how deep we've gone.
        unsigned depth = 1;

        while (1) {
            switch (*m_cur_ptr++) {
            case '*':
                // Check for a '*/'
                if (*m_cur_ptr == '/') {
                    ++m_cur_ptr;

                    if (--depth == 0) {
                        return;
                    }
                }
                break;
            case '/':
                // Check for a '/*'
                if (*m_cur_ptr == '*') {
                    ++m_cur_ptr;
                    ++depth;
                }
                break;

            case '\n':
            case '\r':
                m_next_token.setAtStartOfLine(true);
                break;

            default:
                // If this is a "high" UTF-8 character, validate it.
                if ((signed char)(m_cur_ptr[-1]) < 0) {
                    --m_cur_ptr;

                    const char *char_start = m_cur_ptr;

                    if (validateUTF8CharacterAndAdvance(m_cur_ptr, m_buffer_end) == ~0U) {
                        //@TODO
                        //diagnose(char_start, diag::lex_invalid_utf8);
                    }
                }
                break;   // Otherwise, eat other characters.
            case 0:
                // If this is a random nul character in the middle of a buffer, skip it as
                // whitespace.
                if (m_cur_ptr-1 != m_buffer_end) {
                    //@TODO
                    //diagnoseEmbeddedNul(Diags, m_cur_ptr-1);
                    break;
                }

                // Otherwise, we have an unterminated /* comment.
                --m_cur_ptr;

                // Count how many levels deep we are.
                llvm::SmallString<8> terminator("*/");
                while (--depth != 0) {
                    terminator += "*/";
                }

                const char *EOL = (m_cur_ptr[-1] == '\n') ? (m_cur_ptr - 1) : m_cur_ptr;

                //@TODO
                //diagnose(EOL, diag::lex_unterminated_block_comment)
                //    .fixItInsert(getSourceLoc(EOL), terminator);

                //diagnose(start_ptr, diag::lex_comment_start);

                return;
            }
        }
    }

    void Lexer::tryLexEditorPlaceholder() {
        assert(m_cur_ptr[-1] == '<' && m_cur_ptr[0] == '#');
        const char *token_start = m_cur_ptr-1;

        for (const char *ptr = m_cur_ptr+1; ptr < m_buffer_end-1; ++ptr) {
            if (*ptr == '\n') {
                break;
            }

            if (ptr[0] == '<' && ptr[1] == '#') {
                break;
            }

            if (ptr[0] == '#' && ptr[1] == '>') {
                // Found it. Flag it as error (or warning, if in playground mode) for the
                // rest of the compiler pipeline and lex it as an identifier.

                //@TODO
                //diagnose(token_start, diag::lex_editor_placeholder);

                m_cur_ptr = ptr + 2;

                return formToken(syntax::TokenKind::Identifier, token_start);
            }
        }

        // Not a well-formed placeholder.
        lexOperatorIdentifier();
    }

    bool Lexer::tryLexConflictMarker() {
        const char *ptr = m_cur_ptr - 1;

        // Only a conflict marker if it starts at the beginning of a line.
        if (ptr != m_buffer_start && ptr[-1] != '\n' && ptr[-1] != '\r') {
            return false;
        }

        // Check to see if we have <<<<<<< or >>>>.
        llvm::StringRef rest_of_buffer(ptr, m_buffer_end - ptr);
        if (!rest_of_buffer.startswith("<<<<<<< ") && !rest_of_buffer.startswith(">>>> ")) {
            return false;
        }

        ConflictMarkerKind kind = *ptr == '<' ? ConflictMarkerKind::Normal : ConflictMarkerKind::Perforce;
        if (const char *end = findConflictEnd(ptr, m_buffer_end, kind)) {
            // Diagnose at the conflict marker, then jump ahead to the end.
            //@TODO
            //diagnose(m_cur_ptr, diag::lex_conflict_marker_in_file);
            m_cur_ptr = end;

            // Skip ahead to the end of the marker.
            if (m_cur_ptr != m_buffer_end) {
                skipToEndOfLine();
            }

            return true;
        }

        // No end of conflict marker found.
        return false;
    }

    /// lexDollarIdent - Match $[0-9a-zA-Z_$]+
    void Lexer::lexDollarIdent() {
        const char *token_start = m_cur_ptr-1;
        assert(*token_start == '$');

        // In a SIL function body, '$' is a token by itself.
        if (m_in_sil_body) {
            return formToken(syntax::TokenKind::SilDollar, token_start);
        }

        bool is_all_digits = true;
        for (;; ++m_cur_ptr) {
            if (isDigit(*m_cur_ptr)) {
                // continue
            } else if (clang::isIdentifierHead(*m_cur_ptr, /*dollar*/true)) {
                is_all_digits = false;
                // continue
            } else {
                break;
            }
        }

        if (m_cur_ptr == token_start + 1) {
            return formToken(syntax::TokenKind::Identifier, token_start);
        }

        // We reserve $nonNumeric for persistent bindings in the debugger.
        if (!is_all_digits) {
            //@TODO
            /*if (!LangOpts.EnableDollarIdentifiers) {
                diagnose(tokStart, diag::expected_dollar_numeric);
            }*/

            // Even if we diagnose, we go ahead and form an identifier token,
            // in part to ensure that the basic behavior of the lexer is
            // independent of language mode.
            return formToken(syntax::TokenKind::Identifier, token_start);
        } else {
            return formToken(syntax::TokenKind::DollarIdent, token_start);
        }
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
            case '#': return lexHash();

            // Operator characters.
            case '/':
                if (m_cur_ptr[0] == '/') {  // "//"
                    skipSlashSlashComment();
                    m_seen_comment = true;

                    if (isKeepingComments()) {
                        return formToken(syntax::TokenKind::Comment, token_start);
                    }

                    goto Restart;
                }

                if (m_cur_ptr[0] == '*') { // "/*"
                    skipSlashStarComment();
                    m_seen_comment = true;

                    if (isKeepingComments()) {
                        return formToken(syntax::TokenKind::Comment, token_start);
                    }

                    goto Restart;
                }

                return lexOperatorIdentifier();

            case '%':
                // Lex %[0-9a-zA-Z_]+ as a local SIL value
                if (m_in_sil_body && clang::isIdentifierBody(m_cur_ptr[0])) {
                    do {
                        ++m_cur_ptr;
                    } while (clang::isIdentifierBody(m_cur_ptr[0]));

                    return formToken(syntax::TokenKind::SilLocalName, token_start);
                }

                return lexOperatorIdentifier();

            case '!':
                if (m_in_sil_body) {
                    return formToken(syntax::TokenKind::SilExclamation, token_start);
                }

                if (isLeftBound(token_start, m_buffer_start)) {
                    return formToken(syntax::TokenKind::ExclaimPostfix, token_start);
                }

                return lexOperatorIdentifier();

            case '?':
                if (isLeftBound(token_start, m_buffer_start)) {
                    return formToken(syntax::TokenKind::QuestionPostfix, token_start);
                }

                return lexOperatorIdentifier();

            case '<':
                if (m_cur_ptr[0] == '#') {
                    return tryLexEditorPlaceholder();
                } else if (m_cur_ptr[0] == '<' && tryLexConflictMarker()) {
                    goto Restart;
                }

                return lexOperatorIdentifier();

            case '>':
                if (m_cur_ptr[0] == '>' && tryLexConflictMarker()) {
                    goto Restart;
                }

                return lexOperatorIdentifier();

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
                // std::cout << "Debug: lexIdentifier" << std::endl;
                return lexIdentifier();

            case '$': return lexDollarIdent();

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
