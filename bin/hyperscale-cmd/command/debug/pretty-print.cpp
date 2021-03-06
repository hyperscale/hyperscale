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
#include <hyper/console/value.hpp>
//#include <hyperscale/parser/parser.hpp>
#include <hyperscale/parser/lexer.hpp>
//#include <hyperscale/ast/node.hpp>
#include <hyperscale/ast/pretty-printer-visitor.hpp>
#include <hyperscale/ast/graph-visitor.hpp>
#include <hyperscale/ast/int-expr.hpp>
#include <hyperscale/ast/source-file.hpp>
#include <hyperscale/ast/var-decl.hpp>
#include <hyperscale/ast/op-expr.hpp>
#include <hyperscale/ast/paren-expr.hpp>
#include <hyperscale/ast/call-expr.hpp>
#include <hyperscale/ast/decl-ref-expr.hpp>
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
        addOption(new hyper::console::Value<std::string>("", "format", "The output format", "text"));
    }

    int PrettyPrintCommand::execute() {
        auto args = getArguments();
        auto format = getLongOpt<std::string>("format")->getValue();

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

        auto left1 = hyperscale::parser::Token(hyperscale::syntax::TokenKind::IntegerLiteral);
        left1.setStartOffset(0);
        left1.setLine(1);
        left1.setColumn(1);
        left1.setText(llvm::StringRef("8"));

        auto right2 = hyperscale::parser::Token(hyperscale::syntax::TokenKind::IntegerLiteral);
        right2.setStartOffset(5);
        right2.setLine(1);
        right2.setColumn(6);
        right2.setText(llvm::StringRef("5"));

        auto var = hyperscale::parser::Token(hyperscale::syntax::TokenKind::KeywordVar);
        var.setStartOffset(0);
        var.setLine(1);
        var.setColumn(1);
        var.setText(llvm::StringRef("i"));

        auto sourceFile = new hyperscale::ast::SourceFile();

        sourceFile->addNode(
            new hyperscale::ast::VarDecl(
                var,
                "i",
                new hyperscale::ast::ParenExpr(
                    new hyperscale::ast::OpExpr(
                        new hyperscale::ast::IntExpr(left),
                        hyperscale::ast::Operator::add,
                        new hyperscale::ast::OpExpr(
                            new hyperscale::ast::IntExpr(left1),
                            hyperscale::ast::Operator::mul,
                            new hyperscale::ast::IntExpr(right2)
                        )
                    )
                )
            )
        );

        auto call = hyperscale::parser::Token(hyperscale::syntax::TokenKind::Identifier);
        call.setStartOffset(10);
        call.setLine(3);
        call.setColumn(1);
        call.setText(llvm::StringRef("print"));

        auto argRef = hyperscale::parser::Token(hyperscale::syntax::TokenKind::Identifier);
        argRef.setAtStartOfLine(17);
        argRef.setLine(3);
        argRef.setColumn(9);
        argRef.setText(llvm::StringRef("i"));

        auto callExpr = new hyperscale::ast::CallExpr(call);
        callExpr->addArgument(new hyperscale::ast::DeclRefExpr(argRef));

        sourceFile->addNode(callExpr);

        if (format == "dot") {
            hyperscale::ast::GraphVisitor print(std::cout);

            print(*sourceFile);
        } else {
            hyperscale::ast::PrettyPrinterVisitor print(std::cout);

            print(*sourceFile);
        }

        delete sourceFile;

        return EXIT_SUCCESS;
    }

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
