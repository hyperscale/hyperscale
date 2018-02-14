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
    std::string content = "var i = 1 + 2;";

    hyperscale::parser::Lexer lexer(content);

    BOOST_CHECK_EQUAL(lexer.isEndOfFile(), false);

    auto tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Identifier);
    BOOST_CHECK_EQUAL(tok.getText().str(), "var");

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Identifier);
    BOOST_CHECK_EQUAL(tok.getText().str(), "i");

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Equal);
    BOOST_CHECK_EQUAL(tok.getText().str(), "=");

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::IntegerLiteral);
    BOOST_CHECK_EQUAL(tok.getText().str(), "1");

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Plus);
    BOOST_CHECK_EQUAL(tok.getText().str(), "+");

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::IntegerLiteral);
    BOOST_CHECK_EQUAL(tok.getText().str(), "2");

    tok = lexer.lex();

    BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Semi);
    BOOST_CHECK_EQUAL(tok.getText().str(), ";");
}

BOOST_AUTO_TEST_SUITE_END()