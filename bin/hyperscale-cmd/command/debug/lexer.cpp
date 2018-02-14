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
#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/parser/token.hpp>
#include <iostream>
#include <string>
#include <command/debug/lexer.hpp>

//#include "lexer.hpp"

namespace hyperscale {
namespace command {
namespace debug {

    void LexerCommand::configuration() {
        setName("lexer");
        setDescription("Debug hyperscale lexer");
    }


    int LexerCommand::execute() {
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

        hyperscale::parser::Lexer lexer(content);

        while (!lexer.isEndOfFile()) {
            auto token = lexer.lex();
            std::cout << token << std::endl;
        }

        return EXIT_SUCCESS;
    }

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
