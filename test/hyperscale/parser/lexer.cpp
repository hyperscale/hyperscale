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
#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/syntax/token_kinds.hpp>


BOOST_AUTO_TEST_SUITE(hyperscale_parser_lexer)

BOOST_AUTO_TEST_CASE(test_lexer_lex) {
    std::string content = "var i = (1 + 22);";

    hyperscale::parser::Lexer lexer(content);

    BOOST_CHECK_EQUAL(lexer.isEndOfFile(), false);

    auto tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Identifier);
    BOOST_CHECK_EQUAL(tok.getText().str(), "var");
    BOOST_CHECK_EQUAL(tok.getColumn(), 1);
    BOOST_CHECK_EQUAL(tok.getLine(), 1);

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Identifier);
    BOOST_CHECK_EQUAL(tok.getText().str(), "i");
    BOOST_CHECK_EQUAL(tok.getColumn(), 5);
    BOOST_CHECK_EQUAL(tok.getLine(), 1);

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Equal);
    BOOST_CHECK_EQUAL(tok.getText().str(), "=");
    BOOST_CHECK_EQUAL(tok.getColumn(), 7);
    BOOST_CHECK_EQUAL(tok.getLine(), 1);

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::OpenParen);
    BOOST_CHECK_EQUAL(tok.getText().str(), "(");
    BOOST_CHECK_EQUAL(tok.getColumn(), 9);
    BOOST_CHECK_EQUAL(tok.getLine(), 1);

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::IntegerLiteral);
    BOOST_CHECK_EQUAL(tok.getText().str(), "1");
    BOOST_CHECK_EQUAL(tok.getColumn(), 10);
    BOOST_CHECK_EQUAL(tok.getLine(), 1);

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Plus);
    BOOST_CHECK_EQUAL(tok.getText().str(), "+");
    BOOST_CHECK_EQUAL(tok.getColumn(), 12);
    BOOST_CHECK_EQUAL(tok.getLine(), 1);

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::IntegerLiteral);
    BOOST_CHECK_EQUAL(tok.getText().str(), "22");
    BOOST_CHECK_EQUAL(tok.getColumn(), 14);
    BOOST_CHECK_EQUAL(tok.getLine(), 1);

     tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::CloseParen);
    BOOST_CHECK_EQUAL(tok.getText().str(), ")");
    BOOST_CHECK_EQUAL(tok.getColumn(), 16);
    BOOST_CHECK_EQUAL(tok.getLine(), 1);

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Semi);
    BOOST_CHECK_EQUAL(tok.getText().str(), ";");
    BOOST_CHECK_EQUAL(tok.getColumn(), 17);
    BOOST_CHECK_EQUAL(tok.getLine(), 1);

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Eof);

    BOOST_CHECK_EQUAL(lexer.isEndOfFile(), true);
}

BOOST_AUTO_TEST_SUITE_END()
