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
#include "lib/hyperscale/syntax/invalid-token-exception.hpp"

BOOST_AUTO_TEST_SUITE(hyperscale_syntax_invalid_token_exception)

BOOST_AUTO_TEST_CASE(test_invalid_token_exception) {

    auto left = hyperscale::parser::Token(hyperscale::syntax::TokenKind::IntegerLiteral);
    left.setStartOffset(0);
    left.setLine(1);
    left.setColumn(1);
    left.setText("12");

    hyperscale::syntax::InvalidTokenException ex(left);

    BOOST_CHECK_EQUAL(ex.what(), "invalid token 'IntegerLiteral'");
}

BOOST_AUTO_TEST_SUITE_END()
