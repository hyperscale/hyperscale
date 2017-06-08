/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/command/build.hpp>
#include <hyperscale/console/command.hpp>
#include <memory>

namespace hyperscale {
namespace command {

    std::shared_ptr<console::Command> buildCommand() {
        auto cmd = std::make_shared<console::Command>();

        cmd->name("build");
        cmd->description("Build project");
        cmd->handle([](const console::Command& cmd) {
            return EXIT_SUCCESS;
        });

        return cmd;
    }

} // end of command namespace
} // end of hyperscale namespace
