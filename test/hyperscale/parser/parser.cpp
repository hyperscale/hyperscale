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
#include <hyperscale/parser/parser.hpp>
#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/syntax/token-kinds.hpp>
#include <hyperscale/ast/source-file.hpp>

BOOST_AUTO_TEST_SUITE(hyperscale_parser_parser)

BOOST_AUTO_TEST_CASE(test_parser_parse) {
    //std::string content = "var i = (1 + 22);";
    std::string content = "var i = 1 + 22;";

    auto lexer = std::make_shared<hyperscale::parser::Lexer>(content);

    auto parser = std::make_unique<hyperscale::parser::Parser>(lexer);

    hyperscale::ast::Node* ast = parser->parse();

    hyperscale::ast::SourceFile* sourceFile = static_cast<hyperscale::ast::SourceFile*>(ast);

    BOOST_CHECK_EQUAL(sourceFile->getDecls().size(), 1);
}


BOOST_AUTO_TEST_CASE(test_parser_parse_with_call_expr) {
    //std::string content = "var i = (1 + 22);";
    std::string content = "var i = 1 + 22;\nprint(i);";

    auto lexer = std::make_shared<hyperscale::parser::Lexer>(content);

    auto parser = std::make_unique<hyperscale::parser::Parser>(lexer);

    hyperscale::ast::Node* ast = parser->parse();

    hyperscale::ast::SourceFile* sourceFile = static_cast<hyperscale::ast::SourceFile*>(ast);

    BOOST_CHECK_EQUAL(sourceFile->getDecls().size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()
