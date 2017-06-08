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
#include <hyperscale/command/debug/lexer.hpp>
#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/parser/token.hpp>
#include <iostream>
#include <memory>
#include <string>

namespace hyperscale {
namespace command {
namespace debug {

    std::shared_ptr<console::Command> lexerCommand() {
        auto cmd = std::make_shared<console::Command>();

        //std::unique_ptr<console::Command> cmd(new console::Command);

        cmd->name("lexer");
        cmd->description("Debug hyperscale lexer");
        cmd->handle([](const console::Command& cmd) {
            std::string filename = "../dev/test.ts";

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

            while (lexer.isCodeCompletion()) {
                auto token = lexer.lex();
                std::cout << token << std::endl;
            }

            return EXIT_SUCCESS;
        });

        return cmd;
     }

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
