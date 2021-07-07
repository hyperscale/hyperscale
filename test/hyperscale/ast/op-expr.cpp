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
#include "lib/hyperscale/ast/int-expr.hpp"
#include "lib/hyperscale/ast/op-expr.hpp"
#include <memory>

BOOST_AUTO_TEST_SUITE(hyperscale_ast_op_expr)

BOOST_AUTO_TEST_CASE(test_op_expr) {

    // 12 + 45
    auto left = hyperscale::parser::Token(hyperscale::syntax::TokenKind::IntegerLiteral);
    left.setStartOffset(0);
    left.setLine(1);
    left.setColumn(1);
    left.setText("12");

    auto right = hyperscale::parser::Token(hyperscale::syntax::TokenKind::IntegerLiteral);
    right.setStartOffset(5);
    right.setLine(1);
    right.setColumn(6);
    right.setText("45");

    auto expr = new hyperscale::ast::OpExpr(
        new hyperscale::ast::IntExpr(left),
        hyperscale::ast::Operator::add,
        new hyperscale::ast::IntExpr(right)
    );

    BOOST_CHECK(expr->getOperator() == hyperscale::ast::Operator::add);

    BOOST_CHECK_EQUAL(expr->getLeft()->getLine(), std::size_t(1));
    BOOST_CHECK_EQUAL(expr->getLeft()->getColumn(), std::size_t(1));
    BOOST_CHECK_EQUAL(expr->getRight()->getLine(), std::size_t(1));
    BOOST_CHECK_EQUAL(expr->getRight()->getColumn(), std::size_t(6));

    delete expr;
}

BOOST_AUTO_TEST_SUITE_END()
