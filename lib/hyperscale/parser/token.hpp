/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <map>
#include <ostream>
#include <string>

#include "lib/hyperscale/syntax/token-kinds.hpp"

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
        std::string m_text;

    public:
        Token();

        Token(syntax::TokenKind kind);

        Token(syntax::TokenKind kind, std::string text);

        syntax::TokenKind getKind() const;

        void setKind(syntax::TokenKind kind);

        void setStartOffset(std::size_t offset);

        void setEndOffset(std::size_t offset);

        void setLine(std::size_t line);

        void setColumn(std::size_t column);

        std::size_t getStartOffset() const;

        std::size_t getEndOffset() const;

        std::size_t getLine() const;

        std::size_t getColumn() const;

        /// is/isNot - Predicates to check if this token is a specific kind, as in
        /// "if (Tok.is(tok::l_brace)) {...}".
        bool is(syntax::TokenKind kind) const;

        bool isNot(syntax::TokenKind kind) const;

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

        bool isBinaryOperator() const;

        bool isAnyOperator() const;

        bool isNotAnyOperator() const;

        bool isEllipsis() const;

        bool isNotEllipsis() const;

        /// \brief Determine whether this token occurred at the start of a line.
        bool isAtStartOfLine() const;

        /// \brief Set whether this token occurred at the start of a line.
        void setAtStartOfLine(bool value);

        /// \brief True if this token is an escaped identifier token.
        bool isEscapedIdentifier() const;

        /// \brief Set whether this token is an escaped identifier token.
        void setEscapedIdentifier(bool value);

        bool isContextualKeyword(std::string context_keyword) const;

        /// Return true if this is a contextual keyword that could be the start of a decl.
        bool isContextualDeclKeyword() const;

        bool isContextualPunctuator(std::string context_punc) const;

        /// Determine whether the token can be an argument label.
        ///
        /// This covers all identifiers and keywords except those keywords
        /// used
        bool canBeArgumentLabel() const;

        /// True if the token is an identifier or '_'.
        bool isIdentifierOrUnderscore() const;

        /// True if the token is an OpenParen token that does not start a new line.
        bool isFollowingLParen() const;

        /// True if the token is an l_square token that does not start a new line.
        bool isFollowingLSquare() const;

        /// True if the token is any keyword.
        bool isKeyword() const;

        /// True if the token is any literal.
        bool isLiteral() const;

        bool isPunctuation() const;

        std::size_t getLength() const;

        bool hasComment() const;

        std::string getRawText() const;

        std::string getText() const;

        void setText(std::string text);

        /// \brief Set the token to the specified kind and source range.
        void setToken(syntax::TokenKind K, std::string T, unsigned comment_length = 0, bool multiline_string = false) {
            m_kind = K;
            m_text = T;
            m_comment_length = comment_length;
            m_escaped_identifier = false;
            m_multiline_string = multiline_string;
        }

        bool IsMultilineString() const;
    };

    std::ostream& operator<<(std::ostream& os, const Token& token);

} // end of parser namespace
} // end of hyperscale namespace

// #include "lib/hyperscale/parser/token.hxx>
