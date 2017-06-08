/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include <boost/test/unit_test.hpp>
#include <hyperscale/console/application.hpp>
#include <hyperscale/console/command.hpp>

#define C_TEXT( text ) ((const char*)std::string( text ).c_str())

BOOST_AUTO_TEST_SUITE(hyperscale_console_command)

BOOST_AUTO_TEST_CASE(test_command) {
    // const char *argv[] = {C_TEXT("hyperscale"), C_TEXT("-foo"), C_TEXT("version"), NULL};

    // const int argc = sizeof(argv) / sizeof(char*) - 1;

    std::vector<std::string> args = {"hyperscale", "-l=info", "version"};

    hyperscale::console::Application app;
    app.name("hyperscale");
    app.description("Hyperscale compiler");
    app.command([](){
        auto cmd = std::make_shared<hyperscale::console::Command>();
        cmd->name("version");
        cmd->description("Show the hyperscale version information");
        cmd->handle([](const hyperscale::console::Command& cmd_) {
            return EXIT_SUCCESS;
        });
        return cmd;
    }());
    //app.parse(argc, argv);
    app.parse(args);

    BOOST_CHECK_EQUAL(app.run(), EXIT_SUCCESS);
}

BOOST_AUTO_TEST_SUITE_END()
