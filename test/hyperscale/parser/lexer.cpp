// <hyper/console/command.cpp> -*- C++ -*-

/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include "lib/hyperscale/parser/lexer.hpp"
#include "lib/hyperscale/syntax/token-kinds.hpp"

BOOST_AUTO_TEST_SUITE(hyperscale_parser_lexer)

BOOST_AUTO_TEST_CASE(test_lexer_lex) {
    std::string content = "var i = (1 + 22);";

    hyperscale::parser::Lexer lexer(content);

    BOOST_CHECK_EQUAL(lexer.isEndOfFile(), false);

    auto tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::KeywordVar);
    BOOST_CHECK_EQUAL(tok.getText(), "var");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(1));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Identifier);
    BOOST_CHECK_EQUAL(tok.getText(), "i");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(5));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Equal);
    BOOST_CHECK_EQUAL(tok.getText(), "=");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(7));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::OpenParen);
    BOOST_CHECK_EQUAL(tok.getText(), "(");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(9));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::IntegerLiteral);
    BOOST_CHECK_EQUAL(tok.getText(), "1");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(10));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::KeywordOperator);
    BOOST_CHECK_EQUAL(tok.getText(), "+");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(12));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::IntegerLiteral);
    BOOST_CHECK_EQUAL(tok.getText(), "22");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(14));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

     tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::CloseParen);
    BOOST_CHECK_EQUAL(tok.getText(), ")");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(16));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Semi);
    BOOST_CHECK_EQUAL(tok.getText(), ";");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(17));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Eof);

    BOOST_CHECK_EQUAL(lexer.isEndOfFile(), true);
}

BOOST_AUTO_TEST_CASE(test_lexer_lex_with_zero) {
    std::string content = "var i = 0;";

    hyperscale::parser::Lexer lexer(content);

    BOOST_CHECK_EQUAL(lexer.isEndOfFile(), false);

    auto tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::KeywordVar);
    BOOST_CHECK_EQUAL(tok.getText(), "var");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(1));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Identifier);
    BOOST_CHECK_EQUAL(tok.getText(), "i");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(5));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Equal);
    BOOST_CHECK_EQUAL(tok.getText(), "=");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(7));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::IntegerLiteral);
    BOOST_CHECK_EQUAL(tok.getText(), "0");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(9));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Semi);
    BOOST_CHECK_EQUAL(tok.getText(), ";");
    BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(10));
    BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Eof);

    BOOST_CHECK_EQUAL(lexer.isEndOfFile(), true);
}

BOOST_AUTO_TEST_SUITE_END()
