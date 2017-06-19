/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <command/debug.hpp>
#include <hyperscale/hyperscale.hpp>

namespace hyperscale {
namespace command {

    void DebugCommand::configuration() {
        setName("debug");
        setDescription("Debug project");
        addCommand(new debug::LexerCommand());
    }

} // end of command namespace
} // end of hyperscale namespace
