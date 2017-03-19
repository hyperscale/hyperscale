/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <llvm/ADT/StringRef.h>

namespace hyperscale {
namespace parser {

    enum struct token_t {
        T_UNKNOWN = 0,
        T_EOF,
        T_IDENTIFIER,
        T_OPER_BINARY_UNSPACED,   // "x+y"
        T_OPER_BINARY_SPACED,     // "x + y"
        T_OPER_POSTFIX,
        T_OPER_PREFIX,
        T_INTEGER_LITERAL,
        T_FLOATING_LITERAL,
        T_STRING_LITERAL,
        T_POUND_IF,
        T_POUND_ELSE,
        T_POUND_ELSEIF,
        T_POUND_ENDIF,
        T_COMMENT,

        #define KEYWORD(X) KW_##X,
        #define PUNCTUATOR(X, Y) X,
        #include <hyperscale/parser/tokens.def>

        NUM_TOKENS
    };

    class token
    {
    private:
        token_t m_kind;
        bool m_at_start_of_line;
        llvm::StringRef m_value;
        bool m_escaped_identifier;

    public:
        token(): m_kind(token_t::NUM_TOKENS), m_at_start_of_line(false), m_escaped_identifier(false) {}
        ~token() {}

        token_t getKind() const {
            return m_kind;
        }

        void setKind(token_t kind) {
            m_kind = kind;
        }

        /// is/isNot - Predicates to check if this token is a specific kind, as in
        /// "if (Tok.is(tok::l_brace)) {...}".
        bool is(token_t kind) const {
            return m_kind == kind;
        }

        bool isNot(token_t kind) const {
            return m_kind != kind;
        }

        bool isAny(token_t kind) const {
            return is(kind);
        }

        template <typename ...T>
        bool isAny(token_t K1, token_t K2, T... K) const {
            if (is(K1)) {
                return true;
            }

            return isAny(K2, K...);
        }

        // Predicates to check to see if the token is not the same as any of a list.
        template <typename ...T>
        bool isNot(token_t K1, T... K) const {
            return !isAny(K1, K...);
        }

        bool isBinaryOperator() const {
            return (m_kind == token_t::T_OPER_BINARY_SPACED) || (m_kind == token_t::T_OPER_BINARY_UNSPACED);
        }

        bool isAnyOperator() const {
            return isBinaryOperator() || (m_kind == token_t::T_OPER_POSTFIX) || (m_kind == token_t::T_OPER_PREFIX);
        }

        bool isNotAnyOperator() const {
            return !isAnyOperator();
        }

        bool isEllipsis() const {
            return isAnyOperator() && m_value == "...";
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
            assert((!value || m_kind == token_t::T_IDENTIFIER) && "only identifiers can be escaped identifiers");
            m_escaped_identifier = value;
        }

        bool isContextualKeyword(llvm::StringRef ContextKW) const {
            return is(token_t::T_IDENTIFIER) && !isEscapedIdentifier() && m_value == ContextKW;
        }

        bool isContextualPunctuator(llvm::StringRef ContextPunc) const {
            return isAnyOperator() && m_value == ContextPunc;
        }

        /// True if the token is an identifier or '_'.
        bool isIdentifierOrUnderscore() const {
            return isAny(token_t::T_IDENTIFIER, token_t::KW__);
        }

        /// True if the token is an l_paren token that does not start a new line.
        bool isFollowingLParen() const {
            return !isAtStartOfLine() && m_kind == token_t::l_paren;
        }

        /// True if the token is an l_square token that does not start a new line.
        bool isFollowingLSquare() const {
            return !isAtStartOfLine() && m_kind == token_t::l_square;
        }

        /// True if the token is any keyword.
        bool isKeyword() const {
            switch (m_kind) {
                #define KEYWORD(X) case token_t::KW_##X: return true;
                #include <hyperscale/parser/tokens.def>
                default: return false;
            }
        }

        unsigned getLength() const {
            return m_value.size();
        }

        llvm::StringRef getText() const {
            if (m_escaped_identifier) {
                // Strip off the backticks on either side.
                assert(m_value.front() == '`' && m_value.back() == '`');
                return m_value.slice(1, m_value.size() - 1);
            }

            return m_value;
        }

        void setText(llvm::StringRef value) {
            m_value = value;
        }

        void setToken(token_t kind, llvm::StringRef value) {
            m_kind = kind;
            m_value = value;
            m_escaped_identifier = false;
        }
    };

} // end of parser namespace
} // end of hyperscale namespace

namespace llvm {
    template <typename T>
    struct isPodLike;
    template <>
    struct isPodLike<hyperscale::parser::token> {
        static const bool value = true;
    };
} // end of llvm namespace
