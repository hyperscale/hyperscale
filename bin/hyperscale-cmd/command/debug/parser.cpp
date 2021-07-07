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

#include "lib/hyperscale/ast/pretty-printer-visitor.hpp"
#include "lib/hyperscale/ast/graph-visitor.hpp"
#include "lib/hyperscale/ast/node.hpp"
#include "lib/hyperscale/parser/parser.hpp"
#include "lib/hyperscale/parser/lexer.hpp"

#include "parser.hpp"
#include "hyper/console/value.hpp"

namespace hyperscale {
namespace command {
namespace debug {

    void ParserCommand::configuration() {
        setName("parser");
        setDescription("Debug hyperscale parser");
        addOption(new hyper::console::Value<std::string>("", "format", "The output format", "text"));
    }


    int ParserCommand::execute() {
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

        auto lexer = std::make_shared<hyperscale::parser::Lexer>(content);

        auto parser = std::make_unique<hyperscale::parser::Parser>(lexer);

        hyperscale::ast::Node* ast = parser->parse();

        if (format == "dot") {
            hyperscale::ast::GraphVisitor print(std::cout);

            print(*ast);
        } else {
            hyperscale::ast::PrettyPrinterVisitor print(std::cout);

            print(*ast);
        }

        delete ast;

        return EXIT_SUCCESS;
    }

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
