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
#include <hyperscale/parser/parser.hpp>
#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/ast/node.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <command/debug/parser.hpp>

namespace hyperscale {
namespace command {
namespace debug {

    void ParserCommand::configuration() {
        setName("parser");
        setDescription("Debug hyperscale parser");
    }


    int ParserCommand::execute() {
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

        auto parser = std::make_shared<hyperscale::parser::Parser>(lexer);

        std::shared_ptr<hyperscale::ast::Node> ast = parser->parse();

        std::cout << "Type: " << ast->getType() << std::endl;
        std::cout << "Line: " << ast->getLine() << std::endl;
        std::cout << "Column: " << ast->getColumn() << std::endl;

        return EXIT_SUCCESS;
    }

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
