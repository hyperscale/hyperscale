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
#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/ast/node.hpp>
#include <hyperscale/ast/expr.hpp>

namespace hyperscale {
namespace parser {

    class Parser {
    private:
        // std::unique_ptr<Lexer> m_lexer;
        std::vector<Token> m_tokens;
        std::size_t m_index;

        void expectToken(Token token, syntax::TokenKind kind);

        Token eatToken(syntax::TokenKind kind);

        /*
        VariableDeclaration = ("var" | "const" | "let") Symbol option(":" TypeExpr) "=" Expression
        */
        ast::Node* parseVariableDeclarationExpr(bool mandatory);

        /*
        Expression = ParenExpression | Expression
        */
        ast::Expr* parseExpression(bool mandatory);

        /*
        ParenExpression = "(" Expression ")"
        */
        ast::Expr* parseParenExpression(bool mandatory);

        /*
        OperatorExpression = Expression ("+" | "-" | "*" | "/" | "=" | "<>" | "<" | "<=" | ">" | ">=") Expression
        */
        ast::Expr* parseOperatorExpression(bool mandatory);

        /*
        PrimaryExpression = Integer | Float
        */
        ast::Expr* parsePrimaryExpression(bool mandatory);

    public:
        Parser(std::shared_ptr<Lexer>& lexer);

        ~Parser();

        ast::Node* parse();
    };

} // end of parser namespace
} // end of hyperscale namespace
