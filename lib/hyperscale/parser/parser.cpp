/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <iostream>
#include <hyperscale/parser/parser.hpp>
#include <hyperscale/ast/source-file.hpp>
#include <hyperscale/ast/var-decl.hpp>
#include <hyperscale/ast/op-expr.hpp>
#include <hyperscale/ast/int-expr.hpp>
#include <hyperscale/syntax/expected-token-exception.hpp>
#include <hyperscale/syntax/invalid-token-exception.hpp>

namespace hyperscale {
namespace parser {

    Parser::Parser(std::shared_ptr<Lexer>& lexer): m_index(0) {
        while (!lexer->isEndOfFile()) {
            auto token = lexer->lex();
            m_tokens.push_back(token);

            std::cout << token << std::endl;
        }
    }

    Parser::~Parser() {}

    void Parser::expectToken(Token token, syntax::TokenKind kind) {
        if (token.getKind() == kind) {
            return;
        }

        throw syntax::ExpectedTokenException(token, kind);
    }

    Token Parser::eatToken(syntax::TokenKind kind) {
        auto token = m_tokens[m_index];

        expectToken(token, kind);

        m_index++;

        return token;
    }

    /*
    VariableDeclaration = ("var" | "const" | "let") Identifier option(":" TypeExpr) "=" Expression
    */
    ast::Node* Parser::parseVariableDeclarationExpr(bool mandatory) {
        auto token = m_tokens[m_index];

        bool is_const;

        if (token.is(syntax::TokenKind::KeywordVar)) {
            //@DEBUG
            std::cout << "var";

            is_const = false;
            m_index += 1;
        } else if (token.is(syntax::TokenKind::KeywordLet)) {
            //@DEBUG
            std::cout << "let";

            is_const = true;
            m_index += 1;
        } else if (mandatory) {
            throw syntax::InvalidTokenException(token);
        } else {
            return nullptr;
        }

        auto node = new ast::VarDecl(token);

        token = eatToken(syntax::TokenKind::Identifier);

        //@DEBUG
        std::cout << " " << token.getText();


        node->setType(token.getText());

        token = m_tokens[m_index];

        if (token.is(syntax::TokenKind::Colon)) {
            m_index++;
            //@TODO: parseTypeExpr();

            token = m_tokens[m_index];
        }

        if (token.is(syntax::TokenKind::Equal)) {
            //@DEBUG
            std::cout << " = ";

            m_index++;

            auto expr = parseExpression(true);

            node->setValue(expr);

            token = m_tokens[m_index];
        }

        //@DEBUG
        std::cout << ";" << std::endl;


        expectToken(token, syntax::TokenKind::Semi);

        return node;
    }

    /*
    Expression = ParenExpression | OperatorExpression | IntExpression
    */
    ast::Expr* Parser::parseExpression(bool mandatory) {
        auto token = m_tokens[m_index];

/*
        if (token.is(syntax::TokenKind::OpenParen)) {
            return parseParenExpression(mandatory);
        }
*/

        auto node = parseOperatorExpression(false);
        if (node != nullptr) {
            return node;
        }

        return nullptr;
        //return parsePrimaryExpression(mandatory);
    }

    /*
    ParenExpression = "(" Expression ")"
    */
    ast::Expr* Parser::parseParenExpression(bool mandatory) {
        auto token = m_tokens[m_index];

        expectToken(token, syntax::TokenKind::OpenParen);

        m_index++;

        auto expr = parseExpression(mandatory);

        token = m_tokens[m_index];

        expectToken(token, syntax::TokenKind::CloseParen);

        m_index++;

        return expr;
    }


    /*
    OperatorExpression = Expression ("+" | "-" | "*" | "/" | "=" | "<>" | "<" | "<=" | ">" | ">=") Expression
    */
    ast::Expr* Parser::parseOperatorExpression(bool mandatory) {
        // check PrimaryExpression Operator

        if (m_tokens.size() <= m_index) {
            return nullptr;
        }

        auto token = m_tokens[m_index];
        auto next = m_tokens[m_index+1];

        ast::Expr* left = nullptr;

        if (
            token.isAny(syntax::TokenKind::IntegerLiteral, syntax::TokenKind::FloatingLiteral) &&
            next.is(syntax::TokenKind::KeywordOperator)
        ) {
            left = parsePrimaryExpression(mandatory);
        } /*else if (token.is(syntax::TokenKind::OpenParen)) {
            left = parseParenExpression(mandatory);
        } else {
            left = parseExpression(mandatory);
        }*/

        if (left == nullptr) {
            return nullptr;
        }

        token = m_tokens[m_index];

        expectToken(token, syntax::TokenKind::KeywordOperator);

        auto op = token;


        //@DEBUG
        std::cout << " " << op.getText() << " ";


        if (ast::OperatorNameToOperatorMap.count(op.getText()) == 0) {
            //@TODO: throw exception
            return nullptr;
        }

        m_index++;

        token = m_tokens[m_index];

        ast::Expr* right = nullptr;

        if (
            token.isAny(syntax::TokenKind::IntegerLiteral, syntax::TokenKind::FloatingLiteral)
        ) {
            right = parsePrimaryExpression(mandatory);
        }

        //auto right = parseExpression(mandatory);
        if (right == nullptr) {
            return nullptr;
        }

        return new ast::OpExpr(left, ast::OperatorNameToOperatorMap.at(op.getText()), right);
    }

    /*
    PrimaryExpression = Integer | Float
    KeywordLiteral = "true" | "false"
    */
    ast::Expr* Parser::parsePrimaryExpression(bool mandatory) {
        auto token = m_tokens[m_index];

        //@DEBUG
        std::cout << token.getText();

        if (token.is(syntax::TokenKind::IntegerLiteral)) {
            auto node = new ast::IntExpr(token);
            m_index++;

            return node;
        } else if (token.is(syntax::TokenKind::FloatingLiteral)) {
            //@TODO:
            return nullptr;
        }

        if (!mandatory) {
            return nullptr;
        }

        throw syntax::InvalidTokenException(token);
    }

    ast::Node* Parser::parse() {
        auto sourceFile = new ast::SourceFile();

        while (m_tokens[m_index].isNot(syntax::TokenKind::Eof)) {
            auto decl = parseVariableDeclarationExpr(false);
            if (decl != nullptr) {
                sourceFile->addNode(decl);

                //Hack for test
                return sourceFile;
            }
        }

        return sourceFile;
    }

} // end of parser namespace
} // end of hyperscale namespace
