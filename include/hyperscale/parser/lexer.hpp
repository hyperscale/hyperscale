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
#include <hyperscale/parser/token.hpp>

namespace hyperscale {
namespace parser {

    enum class comment_retention_mode_t {
        NONE,
        ATTACH_TO_NEXT_TOKEN,
        RETURN_AS_TOKENS,
    };

    class lexer
    {
    private:
        const unsigned m_buffer_id;

        /// Pointer to the first character of the buffer, even in a lexer that
        /// scans a subrange of the buffer.
        const char* m_buffer_start;

        /// Pointer to one past the end character of the buffer, even in a lexer
        /// that scans a subrange of the buffer.  Because the buffer is always
        /// NUL-terminated, this points to the NUL terminator.
        const char* m_buffer_end;

        token_t m_next_token;

        const comment_retention_mode_t m_retain_comments;

        lexer(const SourceManager& source_manager, unsigned buffer_id, comment_retention_mode_t retain_comments);

        lexer(const lexer&) = delete;

        void operator=(const lexer&) = delete;

        void primeLexer();

        void lexImpl();

        void skipToEndOfLine();

        /// Skip to the end of the line of a // comment.
        void skipSlashSlashComment();

        /// Skip a #! hashbang line.
        void skipHashbang();

        void skipSlashStarComment();
        void lexIdentifier();
        void lexDollarIdent();
        void lexOperatorIdentifier();
        void lexHexNumber();
        void lexNumber();
        static unsigned lexUnicodeEscape(const char *&CurPtr, lexer *Diags);

        unsigned lexCharacter(const char *&CurPtr, char StopQuote, bool EmitDiagnostics);
        void lexStringLiteral();
        void lexEscapedIdentifier();

        void tryLexEditorPlaceholder();
        const char *findEndOfCurlyQuoteStringLiteral(const char*);
    public:
        /**
         * Create a normal lexer that scans the whole source buffer.
         */
        lexer(const SourceManager& source_manager, unsigned buffer_id, comment_retention_mode_t retain_comments = comment_retention_mode_t::NONE)
            : lexer(source_manager, buffer_id, retain_comments) {
            primeLexer();
        }

        void lex(token_t &result) {
            result = m_next_token;
            if (result.isNot(token_t::T_EOF)) {
                lexImpl();
            }
        }

        /**
         * Get buffer id
         *
         * @return The buffer id
         */
        unsigned getBufferID() const {
            return m_buffer_id;
        }

        bool isKeepingComments() const {
            return m_retain_comments == comment_retention_mode_t::RETURN_AS_TOKENS;
        }

        /**
         * Return the next token to be returned by Lex without actually lexing it.
         * @return
         */
        const token_t& peekNextToken() const {
            return m_next_token;
        }
    };

} // end of parser namespace
} // end of hyperscale namespace
