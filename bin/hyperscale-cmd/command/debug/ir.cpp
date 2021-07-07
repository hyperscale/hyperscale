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
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <hyper/console/value.hpp>

#include "command/debug/ir.hpp"

#include "lib/hyperscale/ast/ir-generator-visitor.hpp"
#include "lib/hyperscale/ast/node.hpp"
#include "lib/hyperscale/ir/ir-generator-module.hpp"
#include "lib/hyperscale/parser/parser.hpp"
#include "lib/hyperscale/parser/lexer.hpp"

namespace hyperscale {
namespace command {
namespace debug {

    void IRCommand::configuration() {
        setName("ir");
        setDescription("Debug hyperscale IR");
    }

    int IRCommand::execute() {
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

        auto lexer = std::make_shared<hyperscale::parser::Lexer>(content);

        auto parser = std::make_unique<hyperscale::parser::Parser>(lexer);

        hyperscale::ast::Node* ast = parser->parse();

        hyperscale::ir::IRGeneratorModule module;

        hyperscale::ast::IRGeneratorVisitor visit(module);

        visit(*ast);

        module.print();

        delete ast;

        return EXIT_SUCCESS;
    }

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
