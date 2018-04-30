/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <exception>
#include <fstream>
//#include <hyperscale/parser/parser.hpp>
#include <hyperscale/parser/lexer.hpp>
//#include <hyperscale/ast/node.hpp>
#include <hyperscale/ast/pretty-printer-visitor.hpp>
#include <hyperscale/ast/int-expr.hpp>
#include <hyperscale/ast/op-expr.hpp>
#include <hyperscale/ast/paren-expr.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <command/debug/pretty-print.hpp>

namespace hyperscale {
namespace command {
namespace debug {

    void PrettyPrintCommand::configuration() {
        setName("pretty-print");
        setDescription("Debug hyperscale pretty print ast");
    }


    int PrettyPrintCommand::execute() {
        auto args = getArguments();

        if (args.empty()) {
            std::cout << "No file" << std::endl;

            return EXIT_SUCCESS;
        }

        std::string filename = args.front();

        std::ifstream file(filename);

        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file");
        }

        std::string content(
            (std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>())
        );
        file.close();

        auto lexer = std::make_unique<hyperscale::parser::Lexer>(content);
/*
        auto parser = std::make_shared<hyperscale::parser::Parser>(lexer);

        std::shared_ptr<hyperscale::ast::Node> ast = parser->parse();

        std::cout << "Type: " << ast->getType() << std::endl;
        std::cout << "Line: " << ast->getLine() << std::endl;
        std::cout << "Column: " << ast->getColumn() << std::endl;
*/

       // 12 + 45
        auto left = hyperscale::parser::Token(hyperscale::syntax::TokenKind::IntegerLiteral);
        left.setStartOffset(0);
        left.setLine(1);
        left.setColumn(1);
        left.setText(llvm::StringRef("12"));

        auto right = hyperscale::parser::Token(hyperscale::syntax::TokenKind::IntegerLiteral);
        right.setStartOffset(5);
        right.setLine(1);
        right.setColumn(6);
        right.setText(llvm::StringRef("45"));

        auto expr = new hyperscale::ast::ParenExpr(
            new hyperscale::ast::OpExpr(
                new hyperscale::ast::IntExpr(left),
                hyperscale::ast::Operator::add,
                new hyperscale::ast::IntExpr(right)
            )
        );

        std::cout << *expr << std::endl;

        delete expr;

        return EXIT_SUCCESS;
    }

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
