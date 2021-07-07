// <hyper/console/command.cpp> -*- C++ -*-

/**
 * Hyperscale
 *
 * (c) 2015-2019 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/test/unit_test.hpp>
#include <memory>

#include "lib/hyperscale/ast/decl-ref-expr.hpp"
#include "lib/hyperscale/ast/call-expr.hpp"

BOOST_AUTO_TEST_SUITE(hyperscale_ast_call_expr)

BOOST_AUTO_TEST_CASE(test_call_expr) {

    // 12 + 45
    auto print = hyperscale::parser::Token(hyperscale::syntax::TokenKind::Identifier);
    print.setStartOffset(0);
    print.setLine(1);
    print.setColumn(1);
    print.setText("print");

    auto var = hyperscale::parser::Token(hyperscale::syntax::TokenKind::Identifier);
    var.setStartOffset(5);
    var.setLine(1);
    var.setColumn(6);
    var.setText("i");

    auto ref = new hyperscale::ast::DeclRefExpr(var);

    auto call = new hyperscale::ast::CallExpr(print);

    call->addArgument(ref);

    BOOST_CHECK_EQUAL(call->getRef(), "print");
    BOOST_CHECK_EQUAL(call->getArguments().at(0), ref);
    BOOST_CHECK_EQUAL(call->getLine(), std::size_t(1));
    BOOST_CHECK_EQUAL(call->getColumn(), std::size_t(1));

    delete call;
}

BOOST_AUTO_TEST_SUITE_END()
