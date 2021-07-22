/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <cassert>
#include "lib/hyperscale/parser/token.hpp"
#include "lib/hyperscale/syntax/token-kinds.hpp"

namespace hyperscale {
namespace parser {

    Token::Token():
        m_kind(syntax::TokenKind::NUM_TOKENS),
        m_start_offset(0),
        m_end_offset(0),
        m_line(0),
        m_column(0),
        m_at_start_of_line(false),
        m_comment_length(0),
        m_escaped_identifier(false) {}

    Token::Token(syntax::TokenKind kind):
        m_kind(kind),
        m_start_offset(0),
        m_end_offset(0),
        m_line(0),
        m_column(0),
        m_at_start_of_line(false),
        m_comment_length(0),
        m_escaped_identifier(false) {}

    Token::Token(syntax::TokenKind kind, std::string text):
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

    syntax::TokenKind Token::getKind() const {
        return m_kind;
    }

    void Token::setKind(syntax::TokenKind kind) {
        m_kind = kind;
    }

    void Token::setStartOffset(std::size_t offset) {
        m_start_offset = offset;
    }

    void Token::setEndOffset(std::size_t offset) {
        m_end_offset = offset;
    }

    void Token::setLine(std::size_t line) {
        m_line = line;
    }

    void Token::setColumn(std::size_t column) {
        m_column = column;
    }

    std::size_t Token::getStartOffset() const {
        return m_start_offset;
    }

    std::size_t Token::getEndOffset() const {
        return m_end_offset;
    }

    std::size_t Token::getLine() const {
        return m_line;
    }

    std::size_t Token::getColumn() const {
        return m_column;
    }

    /// is/isNot - Predicates to check if this token is a specific kind, as in
    /// "if (Tok.is(tok::l_brace)) {...}".
    bool Token::is(syntax::TokenKind kind) const {
        return m_kind == kind;
    }

    bool Token::isNot(syntax::TokenKind kind) const {
        return m_kind != kind;
    }

    bool Token::isBinaryOperator() const {
        return m_kind == syntax::TokenKind::OperBinarySpaced || m_kind == syntax::TokenKind::OperBinaryUnspaced;
    }

    bool Token::isAnyOperator() const {
        return isBinaryOperator() || m_kind == syntax::TokenKind::OperPostfix || m_kind == syntax::TokenKind::OperPrefix;
    }
    bool Token::isNotAnyOperator() const {
        return !isAnyOperator();
    }

    bool Token::isEllipsis() const {
        return isAnyOperator() && m_text == "...";
    }

    bool Token::isNotEllipsis() const {
        return !isEllipsis();
    }

    /// \brief Determine whether this token occurred at the start of a line.
    bool Token::isAtStartOfLine() const {
        return m_at_start_of_line;
    }

    /// \brief Set whether this token occurred at the start of a line.
    void Token::setAtStartOfLine(bool value) {
        m_at_start_of_line = value;
    }

    /// \brief True if this token is an escaped identifier token.
    bool Token::isEscapedIdentifier() const {
        return m_escaped_identifier;
    }

    /// \brief Set whether this token is an escaped identifier token.
    void Token::setEscapedIdentifier(bool value) {
        assert((!value || m_kind == syntax::TokenKind::Identifier) && "only identifiers can be escaped identifiers");

        m_escaped_identifier = value;
    }

    bool Token::isContextualKeyword(std::string context_keyword) const {
        return is(syntax::TokenKind::Identifier) && !isEscapedIdentifier() && m_text == context_keyword;
    }

    /// Return true if this is a contextual keyword that could be the start of a decl.
    bool Token::isContextualDeclKeyword() const {
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

    bool Token::isContextualPunctuator(std::string context_punc) const {
        return isAnyOperator() && m_text == context_punc;
    }

    /// Determine whether the token can be an argument label.
    ///
    /// This covers all identifiers and keywords except those keywords
    /// used
    bool Token::canBeArgumentLabel() const {
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
    bool Token::isIdentifierOrUnderscore() const {
        return isAny(syntax::TokenKind::Identifier, syntax::TokenKind::Keyword_);
    }

    /// True if the token is an OpenParen token that does not start a new line.
    bool Token::isFollowingLParen() const {
        return !isAtStartOfLine() && m_kind == syntax::TokenKind::OpenParen;
    }

    /// True if the token is an l_square token that does not start a new line.
    bool Token::isFollowingLSquare() const {
        return !isAtStartOfLine() && m_kind == syntax::TokenKind::OpenBracket;
    }

    /// True if the token is any keyword.
    bool Token::isKeyword() const {
        switch (m_kind) {
    #define KEYWORD(X) case syntax::TokenKind::Keyword##X: return true;
    #include "lib/hyperscale/syntax/token-kinds.def"
        default: return false;
        }
    }

    /// True if the token is any literal.
    bool Token::isLiteral() const {
        switch(m_kind) {
        case syntax::TokenKind::IntegerLiteral:
        case syntax::TokenKind::FloatingLiteral:
        case syntax::TokenKind::StringLiteral:
            return true;
        default:
            return false;
        }
    }

    bool Token::isPunctuation() const {
        switch (m_kind) {
    #define PUNCTUATOR(Name, Str) case syntax::TokenKind::Name: return true;
    #include "lib/hyperscale/syntax/token-kinds.def"
        default: return false;
        }
    }

    std::size_t Token::getLength() const {
        return m_text.size();
    }

    bool Token::hasComment() const {
        return m_comment_length != 0;
    }

    std::string Token::getRawText() const {
        return m_text;
    }

    std::string Token::getText() const {
        if (m_escaped_identifier) {
            // Strip off the backticks on either side.
            assert(m_text.front() == '`' && m_text.back() == '`');

            return m_text.substr(1, m_text.size() - 2);
        }

        return m_text;
    }

    void Token::setText(std::string text) {
        m_text = text;
    }

    bool Token::IsMultilineString() const {
        return m_multiline_string;
    }


    std::ostream& operator<<(std::ostream& os, const Token& token) {
        return os << "Token {"
            << "kind: " << static_cast<std::underlying_type<syntax::TokenKind>::type>(token.getKind()) << ", "
            << "value: " << token.getText() << ", "
            << "start: " << token.getStartOffset() << ", "
            << "end: " << token.getEndOffset() << ", "
            << "length: " << token.getLength() << ", "
            << "line: " << token.getLine() << ", "
            << "column: " << token.getColumn()
            << "}" << std::endl;
    }

} // end of parser namespace
} // end of hyperscale namespace



