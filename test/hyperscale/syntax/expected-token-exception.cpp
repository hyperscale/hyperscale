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
#include <hyperscale/syntax/expected-token-exception.hpp>

BOOST_AUTO_TEST_SUITE(hyperscale_syntax_expected_token_exception)

BOOST_AUTO_TEST_CASE(test_expected_token_exception) {

    auto left = hyperscale::parser::Token(hyperscale::syntax::TokenKind::IntegerLiteral);
    left.setStartOffset(0);
    left.setLine(1);
    left.setColumn(1);
    left.setText("12");

    hyperscale::syntax::ExpectedTokenException ex(left, hyperscale::syntax::TokenKind::StringLiteral);

    BOOST_CHECK_EQUAL(ex.what(), "expected token 'StringLiteral'");
}

BOOST_AUTO_TEST_SUITE_END()
