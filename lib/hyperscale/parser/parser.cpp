/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <vector>
#include <hyperscale/ast/node.hpp>
#include <hyperscale/parser/parser.hpp>
#include <hyperscale/parser/token.hpp>
#include <hyperscale/syntax/token_kind.hpp>

namespace hyperscale {
namespace parser {

    Parser::Parser(std::vector<Token> tokens): m_tokens(tokens), m_index(0) {
    }

    Token Parser::nextToken() {
        m_index++;

        return m_tokens.at(m_index);
    }

    void Parser::expectToken(Token token, ast::TokenKind kind) {
        if (!token.is(kind)) {
            // throw error
        }
    }

    std::shared_ptr<ast::Node> Parser::parseExpression() {

    }

    std::shared_ptr<ast::Node> Parser::parseAssignmentExpression() {

    }

    // VariableDeclaration = ("var" | "let") Symbol option(":" TypeExpr) "=" Expression
    std::shared_ptr<ast::Node> Parser::parseVariableDeclarationExpr(bool mandatory) {
        auto token = m_tokens.at(m_index);

        auto node = std::make_shared<ast::Node>(ast::NodeTypeVariableDeclaration, token);
        node.data = std::shared_ptr<NodeVariableDeclaration>();

        if (token.isNot(syntax::TokenKind::KeywordVar, syntax::TokenKind::KeywordLet)) {
            if (mandatory) {
                std::cerr << "Expected token: var or let" << std::endl;
                // throw error
            }

            return nullptr;
        }

        if (token.is(syntax::TokenKind::KeywordLet)) {
            node.data->setConst(true);
        }

        token = nextToken();

        if (!token.is(syntax::TokenKind::Identifier)) {
            std::cerr << "Expected token: Identifier" << std::endl;
            // throw error

            return nullptr;
        }

        node.data->setSymbol(token.getText());

        token = nextToken();

        if (token.is(syntax::TokenKind::Colon)) {
            token = nextToken();

            if (token.is(syntax::TokenKind::Identifier)) {
                std::cerr << "Expected token: Identifier" << std::endl;
                // throw error

                return nullptr;
            }

            node.data->setType(token.getText());
        }

        token = nextToken();

        if (token.is(syntax::TokenKind::Equal)) {
            token = nextToken();

            auto expr = parseExpression()
            if (type.get() == nullptr) {
                std::cerr << "Expected token: expression" << std::endl;
                // throw error

                return nullptr;
            }

            node.data->setExpression(expr);
        }

        token = nextToken();

        expectToken(token, syntax::TokenKind::Semi);

        return node;
    }

    std::shared_ptr<hyperscale::ast::Node> Parser::parse() {
        auto root = std::make_shared<hyperscale::ast::Node>(ast::NodeTypeRoot);
        root.data = std::make_shared<hyperscale::ast::NodeRoot>();

        for (; m_index < m_tokens.size(); m_tokens++) {
            auto node = parseVariableDeclarationExpr(false);
            if (node.get() != nullptr) {
                root.data->addNode(node);

                continue;
            }
        }

        return root;
    }

} // end of parser namespace
} // end of hyperscale namespace
