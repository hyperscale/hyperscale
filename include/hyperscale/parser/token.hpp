/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/syntax/token_kinds.hpp>
#include <llvm/ADT/StringRef.h>
#include <map>
#include <ostream>
#include <string>

namespace hyperscale {
namespace parser {

    class Token {
    private:
        /// Kind - The actual flavor of token this is.
        syntax::TokenKind m_kind;

        std::size_t m_start_offset;

        std::size_t m_end_offset;

        std::size_t m_line;

        std::size_t m_column;

        /// \brief Whether this token is the first token on the line.
        unsigned m_at_start_of_line : 1;

        /// \brief The length of the comment that precedes the token.
        ///
        /// Hopefully 128 Mib is enough.
        unsigned m_comment_length : 27;

        /// \brief Whether this token is an escaped `identifier` token.
        unsigned m_escaped_identifier : 1;

        /// Modifiers for string literals
        unsigned m_multiline_string : 1;

        /// Text - The actual string covered by the token in the source buffer.
        llvm::StringRef m_text;

    public:
        Token():
            m_kind(syntax::TokenKind::NUM_TOKENS),
            m_start_offset(0),
            m_end_offset(0),
            m_line(0),
            m_column(0),
            m_at_start_of_line(false),
            m_comment_length(0),
            m_escaped_identifier(false) {}

        Token(syntax::TokenKind kind):
            m_kind(kind),
            m_start_offset(0),
            m_end_offset(0),
            m_line(0),
            m_column(0),
            m_at_start_of_line(false),
            m_comment_length(0),
            m_escaped_identifier(false) {}

        Token(syntax::TokenKind kind, llvm::StringRef text):
            m_kind(kind),
            m_start_offset(0),
            m_end_offset(0),
            m_line(0),
            m_column(0),
            m_at_start_of_line(false),
            m_comment_length(0),
            m_escaped_identifier(false),
            m_multiline_string(false),
            m_text(text) {}

        syntax::TokenKind getKind() const {
            return m_kind;
        }

        void setKind(syntax::TokenKind kind) {
            m_kind = kind;
        }

        void setStartOffset(std::size_t offset) {
            m_start_offset = offset;
        }

        void setEndOffset(std::size_t offset) {
            m_end_offset = offset;
        }

        void setLine(std::size_t line) {
            m_line = line;
        }

        void setColumn(std::size_t column) {
            m_column = column;
        }

        std::size_t getStartOffset() const {
            return m_start_offset;
        }

        std::size_t getEndOffset() const {
            return m_end_offset;
        }

        std::size_t getLine() const {
            return m_line;
        }

        std::size_t getColumn() const {
            return m_column;
        }

        /// is/isNot - Predicates to check if this token is a specific kind, as in
        /// "if (Tok.is(tok::l_brace)) {...}".
        bool is(syntax::TokenKind kind) const {
            return m_kind == kind;
        }

        bool isNot(syntax::TokenKind kind) const {
            return m_kind != kind;
        }

        // Predicates to check to see if the token is any of a list of tokens.
        bool isAny(syntax::TokenKind kind) const {
            return is(kind);
        }

        template <typename ...T>
        bool isAny(syntax::TokenKind K1, syntax::TokenKind K2, T... K) const {
            if (is(K1)) {
                return true;
            }

            return isAny(K2, K...);
        }

        // Predicates to check to see if the token is not the same as any of a list.
        template <typename ...T>
        bool isNot(syntax::TokenKind K1, T... K) const {
            return !isAny(K1, K...);
        }

        bool isBinaryOperator() const {
            return m_kind == syntax::TokenKind::OperBinarySpaced || m_kind == syntax::TokenKind::OperBinaryUnspaced;
        }

        bool isAnyOperator() const {
            return isBinaryOperator() || m_kind == syntax::TokenKind::OperPostfix || m_kind == syntax::TokenKind::OperPrefix;
        }
        bool isNotAnyOperator() const {
            return !isAnyOperator();
        }

        bool isEllipsis() const {
            return isAnyOperator() && m_text == "...";
        }

        bool isNotEllipsis() const {
            return !isEllipsis();
        }

        /// \brief Determine whether this token occurred at the start of a line.
        bool isAtStartOfLine() const {
            return m_at_start_of_line;
        }

        /// \brief Set whether this token occurred at the start of a line.
        void setAtStartOfLine(bool value) {
            m_at_start_of_line = value;
        }

        /// \brief True if this token is an escaped identifier token.
        bool isEscapedIdentifier() const {
            return m_escaped_identifier;
        }

        /// \brief Set whether this token is an escaped identifier token.
        void setEscapedIdentifier(bool value) {
            assert((!value || m_kind == syntax::TokenKind::Identifier) && "only identifiers can be escaped identifiers");

            m_escaped_identifier = value;
        }

        bool isContextualKeyword(llvm::StringRef context_keyword) const {
            return is(syntax::TokenKind::Identifier) && !isEscapedIdentifier() && m_text == context_keyword;
        }

        /// Return true if this is a contextual keyword that could be the start of a decl.
        bool isContextualDeclKeyword() const {
            if (isNot(syntax::TokenKind::Identifier) || isEscapedIdentifier() || m_text.empty()) {
                return false;
            }

            switch (m_text[0]) {
            case 'c':
                return m_text == "convenience";
            case 'd':
                return m_text == "dynamic";
            case 'f':
                return m_text == "final";
            case 'i':
                return m_text == "indirect" || m_text == "infix";
            case 'l':
                return m_text == "lazy";
            case 'm':
                return m_text == "mutating";
            case 'n':
                return m_text == "nonmutating";
            case 'o':
                return m_text == "open" || m_text == "override" || m_text == "optional";
            case 'p':
                return m_text == "prefix" || m_text == "postfix";
            case 'r':
                return m_text == "required";
            case 'u':
                return m_text == "unowned";
            case 'w':
                return m_text == "weak";
            default:
                return false;
            }
        }

        bool isContextualPunctuator(llvm::StringRef context_punc) const {
            return isAnyOperator() && m_text == context_punc;
        }

        /// Determine whether the token can be an argument label.
        ///
        /// This covers all identifiers and keywords except those keywords
        /// used
        bool canBeArgumentLabel() const {
            // Identifiers, escaped identifiers, and '_' can be argument labels.
            if (is(syntax::TokenKind::Identifier) || isEscapedIdentifier() || is(syntax::TokenKind::Keyword_)) {
                return true;
            }

            // 'let', 'var', and 'inout' cannot be argument labels.
            if (isAny(syntax::TokenKind::KeywordLet, syntax::TokenKind::KeywordVar, syntax::TokenKind::KeywordInout)) {
                return false;
            }

            // All other keywords can be argument labels.
            return isKeyword();
        }

        /// True if the token is an identifier or '_'.
        bool isIdentifierOrUnderscore() const {
            return isAny(syntax::TokenKind::Identifier, syntax::TokenKind::Keyword_);
        }

        /// True if the token is an OpenParen token that does not start a new line.
        bool isFollowingLParen() const {
            return !isAtStartOfLine() && m_kind == syntax::TokenKind::OpenParen;
        }

        /// True if the token is an l_square token that does not start a new line.
        bool isFollowingLSquare() const {
            return !isAtStartOfLine() && m_kind == syntax::TokenKind::OpenBracket;
        }

        /// True if the token is any keyword.
        bool isKeyword() const {
            switch (m_kind) {
        #define KEYWORD(X) case syntax::TokenKind::Keyword##X: return true;
        #include "hyperscale/syntax/token_kinds.def"
            default: return false;
            }
        }

        /// True if the token is any literal.
        bool isLiteral() const {
            switch(m_kind) {
            case syntax::TokenKind::IntegerLiteral:
            case syntax::TokenKind::FloatingLiteral:
            case syntax::TokenKind::StringLiteral:
                return true;
            default:
                return false;
            }
        }

        bool isPunctuation() const {
            switch (m_kind) {
        #define PUNCTUATOR(Name, Str) case syntax::TokenKind::Name: return true;
        #include "hyperscale/syntax/token_kinds.def"
            default: return false;
            }
        }

        unsigned getLength() const {
            return m_text.size();
        }

        bool hasComment() const {
            return m_comment_length != 0;
        }

        llvm::StringRef getRawText() const {
            return m_text;
        }

        llvm::StringRef getText() const {
            if (m_escaped_identifier) {
                // Strip off the backticks on either side.
                assert(m_text.front() == '`' && m_text.back() == '`');
                return m_text.slice(1, m_text.size() - 1);
            }

            return m_text;
        }

        void setText(llvm::StringRef T) {
            m_text = T;
        }

        /// \brief Set the token to the specified kind and source range.
        void setToken(syntax::TokenKind K, llvm::StringRef T, unsigned comment_length = 0, bool multiline_string = false) {
            m_kind = K;
            m_text = T;
            m_comment_length = comment_length;
            m_escaped_identifier = false;
            m_multiline_string = multiline_string;
        }

        bool IsMultilineString() const {
            return m_multiline_string;
        }
    };

    std::ostream& operator<<(std::ostream& os, const Token& token);

} // end of parser namespace
} // end of hyperscale namespace
