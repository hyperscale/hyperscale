/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/command/debug.hpp>
#include <hyperscale/command/debug/lexer.hpp>
#include <hyperscale/console/command.hpp>
#include <memory>

namespace hyperscale {
namespace command {

    std::shared_ptr<console::Command> debugCommand() {
        auto cmd = std::make_shared<console::Command>();

        cmd->name("debug");
        cmd->description("Debug hyperscale compiler");
        cmd->command(debug::lexerCommand());

        return cmd;
    }

} // end of command namespace
} // end of hyperscale namespace
