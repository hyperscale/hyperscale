/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/parser/token.hpp>
#include <hyperscale/syntax/token_kinds.hpp>
#include <istream>
#include <string>
#include <vector>

namespace hyperscale {
namespace parser {

    /// Given a pointer to the starting byte of a UTF8 character, validate it and
    /// advance the lexer past it.  This returns the encoded character or ~0U if
    /// the encoding is invalid.
    uint32_t validateUTF8CharacterAndAdvance(const char *&ptr, const char *end);

    class Lexer {
        private:
        const std::string& m_buffer;

        /// Pointer to the first character of the buffer, even in a lexer that
        /// scans a subrange of the buffer.
        const char *m_buffer_start;

        /// Pointer to one past the end character of the buffer, even in a lexer
        /// that scans a subrange of the buffer.  Because the buffer is always
        /// NUL-terminated, this points to the NUL terminator.
        const char *m_buffer_end;

        /// Pointer to the next not consumed character.
        const char *m_cur_ptr;

        Token m_next_token;

        bool m_in_sil_mode;

        /// If non-null, points to the '\0' character in the buffer where we should
        /// produce a code completion token.
        const char *m_code_completion_ptr = nullptr;

        void formToken(const hyperscale::syntax::TokenKind kind, const char *token_start, bool multiline_string = false);

        void lexOperatorIdentifier();

        void lexIdentifier();

        void lexNumber();

        void lexHexNumber();

        void lexStringLiteral();

        void lexEscapedIdentifier();

        void lexImpl();

        syntax::TokenKind kindOfIdentifier(llvm::StringRef str, bool in_sil_mode);

        bool isOperator(llvm::StringRef string);

        bool isIdentifier(llvm::StringRef string);

        public:
        Lexer(const std::string &buffer);


        /// \brief Returns true if this lexer will produce a code completion token.
        bool isCodeCompletion() const {
            return m_code_completion_ptr != nullptr;
        }

        Token lex();

    };



} // end of parser namespace
} // end of hyperscale namespace
