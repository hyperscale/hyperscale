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

#include "lib/hyperscale/parser/lexer.hpp"
#include "lib/hyperscale/parser/token.hpp"

#include "lexer.hpp"
#include "../../console/table.hpp"

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

        console::Table t;
        t.setStyle(console::Table::Style::LIGHT);
        t.setAlignment(3, console::Table::Alignment::RIGHT);

        t.add("#");
        t.add("token name");
        t.add("token value");
        t.add("offset");
        t.endOfRow();

        std::size_t i = 0;

        while (!lexer.isEndOfFile()) {
            auto token = lexer.lex();

            t.add(std::to_string(i));
            t.add(hyperscale::syntax::TokenNames.at(token.getKind()));
            t.add(token.getText());
            t.add(std::to_string(token.getStartOffset()));
            t.endOfRow();

            i++;
        }

        std::cout << t;

        return EXIT_SUCCESS;
    }

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
