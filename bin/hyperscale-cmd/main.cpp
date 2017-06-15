/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyper/console/application.hpp>
#include <hyperscale/config.hpp>

int main(int argc, char **argv) {
    hyper::console::Application* app = new hyper::console::Application();

    app->setName("hyperscale");
    app->setDescription("Hyperscale compiler");
    app->addCommand(new VersionCommand());

    app->parse(argc, argv);

    int code = app->run();

    delete app;

    return code;
}
