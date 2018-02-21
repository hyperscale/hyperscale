/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <vector>
#include <memory>
#include <hyperscale/ast/node.hpp>
#include <hyperscale/parser/token.hpp>
#include <hyperscale/parser/lexer.hpp>

namespace hyperscale {
namespace parser {

    class Parser {
        std::unique_ptr<Lexer> m_lexer;

        Token m_current_token;

        std::shared_ptr<ast::Node> parseVariableDeclarationExpr(bool mandatory);
        std::shared_ptr<ast::Node> parseExpression();
        std::shared_ptr<ast::Node> parseBinaryOperatorExpression();

        void nextToken();

        void eat(syntax::TokenKind kind);

        void error();

        void expectToken(syntax::TokenKind kind);
    public:
        Parser(std::unique_ptr<Lexer>& lexer);

        std::shared_ptr<ast::Node> parse();
    };

} // end of parser namespace
} // end of hyperscale namespace
