/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/command/debug.hpp>
#include <hyperscale/command/debug/lexer.hpp>
#include <hyperscale/command/version.hpp>
#include <hyperscale/config.hpp>
#include <hyperscale/console/application.hpp>

int main(int argc, char **argv) {
    hyperscale::console::Application* app = new hyperscale::console::Application();

    return app->name("hyperscale")
        .description("Hyperscale compiler")
        .command(hyperscale::command::debugCommand())
        .command(hyperscale::command::versionCommand())
        .parse(argc, argv)
        .run();
}
