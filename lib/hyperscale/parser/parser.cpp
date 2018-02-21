/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <hyperscale/ast/node.hpp>
#include <hyperscale/parser/parser.hpp>
#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/parser/token.hpp>

namespace hyperscale {
namespace parser {

    Parser::Parser(std::unique_ptr<Lexer>& lexer): m_lexer(std::move(lexer)) {
        m_current_token = m_lexer->lex();
    }

    void Parser::eat(syntax::TokenKind kind) {
        if (m_current_token.is(kind)) {
            m_current_token = m_lexer->lex();
        } else {
            error();
        }
    }

    void Parser::error() {
        std::ostringstream err;
        err << "Invalid syntax";
        err << " on line " << m_current_token.getLine();
        err << " and col " << m_current_token.getColumn();

        throw std::runtime_error(err.str());
    }

    void Parser::nextToken() {
        m_current_token = m_lexer->lex();
    }

    void Parser::expectToken(syntax::TokenKind kind) {
        if (!m_current_token.is(kind)) {
            std::ostringstream err;
            err << "Expected token: " << kind;
            err << " on line " << m_current_token.getLine();
            err << " and col " << m_current_token.getColumn();

            throw std::runtime_error(err.str());
        }
    }

    std::shared_ptr<ast::Node> Parser::parseExpression() {
        auto binOp = parseBinaryOperatorExpression();
        if (binOp.get() != nullptr) {
            return binOp;
        }


        return nullptr;
    }

    std::shared_ptr<ast::Node> Parser::parseBinaryOperatorExpression() {
        auto node = std::make_shared<ast::Node>(ast::NodeTypeGroupedExpr, m_current_token);

        auto left = parseExpression();
        if (left.get() == nullptr) {
            return nullptr;
        }

        node->getData<ast::NodeBinaryOperatorExpression>()->setLeft(left);

        expectToken(syntax::TokenKind::KeywordOperator);
        // Operator = "+" | "-" | "*" | "/" | "%"
        node->getData<ast::NodeBinaryOperatorExpression>()->setOperator(m_current_token.getText());

        auto right = parseExpression();
        if (right.get() == nullptr) {
            return nullptr;
        }

        node->getData<ast::NodeBinaryOperatorExpression>()->setRight(right);

        return node;
    }

    // VariableDeclaration = ("var" | "let") Symbol option(":" TypeExpr) "=" Expression
    std::shared_ptr<ast::Node> Parser::parseVariableDeclarationExpr(bool mandatory) {
        if (m_current_token.isNot(syntax::TokenKind::KeywordVar, syntax::TokenKind::KeywordLet)) {
            if (mandatory) {
                std::cerr << "Expected token: var or let" << std::endl;
                // throw error
            }

            return nullptr;
        }

        auto node = std::make_shared<ast::Node>(ast::NodeTypeVariableDeclaration, m_current_token);

        if (m_current_token.is(syntax::TokenKind::KeywordLet)) {
            node->getData<ast::NodeVariableDeclaration>()->setConst(true);
        }

        nextToken();
        expectToken(syntax::TokenKind::Identifier);

        node->getData<ast::NodeVariableDeclaration>()->setSymbol(m_current_token.getText());

        nextToken();

        if (m_current_token.is(syntax::TokenKind::Colon)) {
            nextToken();
            expectToken(syntax::TokenKind::Identifier);

            node->getData<ast::NodeVariableDeclaration>()->setType(m_current_token.getText());
        }

        nextToken();

        if (m_current_token.is(syntax::TokenKind::Equal)) {
            nextToken();

            auto expr = parseExpression();
            if (expr.get() == nullptr) {
                std::cerr << "Expected token: expression" << std::endl;
                // throw error

                return nullptr;
            }

            node->getData<ast::NodeVariableDeclaration>()->setExpression(expr);
        }

        nextToken();
        expectToken(syntax::TokenKind::Semi);

        return node;
    }

    std::shared_ptr<ast::Node> Parser::parse() {
        auto root = std::make_shared<ast::Node>(ast::NodeTypeRoot);

        do {
            auto node = parseVariableDeclarationExpr(false);
            if (node.get() != nullptr) {
                root->getData<ast::NodeRoot>()->addNode(node);

                continue;
            }
        } while (m_current_token.isNot(syntax::TokenKind::Eof));

        return root;
    }

} // end of parser namespace
} // end of hyperscale namespace
