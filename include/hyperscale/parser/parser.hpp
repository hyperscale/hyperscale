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

namespace hyperscale {
namespace parser {

    class Parser {
        std::vector<Token> m_tokens;
        std::size_t m_index;

        std::shared_ptr<ast::Node> parseVariableDeclarationExpr();
        std::shared_ptr<ast::Node> parseExpression();
        std::shared_ptr<ast::Node> parseAssignmentExpression();

        Token nextToken();

        void expectToken(Token token, ast::TokenKind kind);
    public:
        Parser(std::vector<Token> tokens);

        std::shared_ptr<ast::Node> parse();
    };

} // end of parser namespace
} // end of hyperscale namespace
