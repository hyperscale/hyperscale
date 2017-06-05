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

    namespace po = boost::program_options;

    int lexer(po::variables_map &vm, po::parsed_options &options, po::options_description &desc) {
        /*po::options_description lexer_desc("debug:lexer options");
        lexer_desc.add_options()
            ("help", "Show help");

        po::positional_options_description p;
        p.add("input-file", 2);

        std::vector<std::string> opts = po::collect_unrecognized(options.options, po::include_positional);
        opts.erase(opts.begin());

        // Parse again...
        po::store(po::command_line_parser(opts).options(lexer_desc).positional(p).run(), vm);

        if (!vm.count("input-file")) {
            throw po::invalid_option_value("input-file");
        }*/

        // std::string filename = vm["input-file"].as<std::string>();

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
    }

    std::shared_ptr<console::Command> lexerCommand() {
        auto cmd = std::make_shared<console::Command>();

        //std::unique_ptr<console::Command> cmd(new console::Command);

        cmd->name("lexer");
        cmd->description("Debug hyperscale lexer");
        cmd->handle([]() {
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
        });

        return cmd;
     }

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
