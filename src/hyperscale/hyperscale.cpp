/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <boost/program_options.hpp>
#include <einfo.h>
#include <hyperscale/command.hpp>
#include <hyperscale/command/build.hpp>
#include <hyperscale/command/debug.hpp>
#include <hyperscale/command/debug/lexer.hpp>
#include <hyperscale/command/help.hpp>
#include <hyperscale/command/version.hpp>
#include <hyperscale/config.hpp>
#include <hyperscale/console/application.hpp>
#include <hyperscale/hyperscale.hpp>
#include <iostream>
#include <map>
#include <string>

//@TODO use struct with desc for help ?
static std::map<std::string, hyperscale::command_t> commands = {
    {"debug:lexer", hyperscale::command::debug::lexer},
    {"build", hyperscale::command::build},
    {"help", hyperscale::command::help},
    {"version", hyperscale::command::version},
};

int main(int argc, char **argv) {

    hyperscale::console::Application* app = new hyperscale::console::Application();

    return app->name("hyperscale")
        .description("Hyperscale compiler")
        .command(hyperscale::command::debugCommand())
        .command(hyperscale::command::versionCommand())
        .parse(argc, argv)
        .run();

    try {
        namespace po = boost::program_options;

        po::options_description desc("Options");
        desc.add_options()
            ("help,h", "Display available options")
            ("verbose,v", "Show commands to run and use verbose output")
            ("version", "Print version information and exit")
            ("command", po::value<std::string>(), "command to execute");
            //("build", po::value<int>(&build), "Compile packages and dependencies")
            //("test", po::value<int>(&test), "Test packages")
            //("lint", "Lint packages");

        po::positional_options_description positional;
        positional.add("command", 1);

        po::variables_map vm;

        po::parsed_options options = po::command_line_parser(argc, argv)
            .options(desc)
            .positional(positional)
            .allow_unregistered()
            .run();

        po::store(options, vm);

        if (vm.count("help")) {
            return hyperscale::command::help(vm, options, desc);
        }

        po::notify(vm);

        if (vm.count("version")) {
            return hyperscale::command::version(vm, options, desc);
        }

        if (vm.count("verbose")) {
            std::cout << "VERBOSE PRINTING" << std::endl;
        }

        std::string cmd = "help";

        if (vm.count("command")) {
            cmd = vm["command"].as<std::string>();
        }

        //@TODO add support of external cmd (hyperscale-*)
        if (commands.count(cmd)) {
            return commands[cmd](vm, options, desc);
        } else {
            throw po::invalid_option_value(cmd);
        }

    } catch (boost::program_options::required_option& e) {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;

        return EXIT_FAILURE;
    } catch (boost::program_options::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;

        return EXIT_FAILURE;
    } catch (std::exception& e) {
        std::cerr << "Unhandled Exception reached the top of main: " << e.what() << ", application will now exit" << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
