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
#include "lib/hyperscale/parser/token.hpp"
#include "lib/hyperscale/syntax/token-kinds.hpp"

BOOST_AUTO_TEST_SUITE(hyperscale_parser_token)

BOOST_AUTO_TEST_CASE(test_token) {
    {
        hyperscale::parser::Token tok(hyperscale::syntax::TokenKind::IntegerLiteral);

        tok.setText("1");
        tok.setColumn(1);
        tok.setLine(1);
        tok.setStartOffset(0);
        tok.setAtStartOfLine(true);

        BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::IntegerLiteral);
        BOOST_CHECK_EQUAL(tok.getText(), "1");
        BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(1));
        BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));
        BOOST_CHECK_EQUAL(tok.getStartOffset(), std::size_t(0));
        BOOST_TEST(tok.is(hyperscale::syntax::TokenKind::IntegerLiteral));
        BOOST_TEST(tok.isNot(hyperscale::syntax::TokenKind::Identifier));
        BOOST_TEST(tok.isAtStartOfLine());
    }

    {
        hyperscale::parser::Token tok(hyperscale::syntax::TokenKind::IntegerLiteral, "1");

        tok.setColumn(1);
        tok.setLine(1);
        tok.setStartOffset(0);

        BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::IntegerLiteral);
        BOOST_CHECK_EQUAL(tok.getText(), "1");
        BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(1));
        BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));
        BOOST_CHECK_EQUAL(tok.getStartOffset(), std::size_t(0));
        BOOST_TEST(tok.is(hyperscale::syntax::TokenKind::IntegerLiteral));
        BOOST_TEST(tok.isNot(hyperscale::syntax::TokenKind::Identifier));
    }

    {
        hyperscale::parser::Token tok(hyperscale::syntax::TokenKind::Identifier, "`foo`");
        tok.setEscapedIdentifier(true);
        tok.setColumn(1);
        tok.setLine(1);
        tok.setStartOffset(0);

        BOOST_CHECK_EQUAL(tok.getKind(), hyperscale::syntax::TokenKind::Identifier);
        BOOST_CHECK_EQUAL(tok.getText(), "foo");
        BOOST_CHECK_EQUAL(tok.getColumn(), std::size_t(1));
        BOOST_CHECK_EQUAL(tok.getLine(), std::size_t(1));
        BOOST_CHECK_EQUAL(tok.getStartOffset(), std::size_t(0));
        BOOST_TEST(tok.is(hyperscale::syntax::TokenKind::Identifier));
        BOOST_TEST(tok.isNot(hyperscale::syntax::TokenKind::StringLiteral));
        BOOST_TEST(tok.isEscapedIdentifier());
    }
}

BOOST_AUTO_TEST_SUITE_END()
