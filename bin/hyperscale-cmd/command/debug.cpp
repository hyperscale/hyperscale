/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "lib/hyperscale/hyperscale.hpp"

#include "debug.hpp"

namespace hyperscale {
namespace command {

    void DebugCommand::configuration() {
        setName("debug");
        setDescription("Debug project");
        addCommand(new debug::LexerCommand());
        addCommand(new debug::ParserCommand());
        addCommand(new debug::PrettyPrintCommand());
        addCommand(new debug::IRCommand());
    }

} // end of command namespace
} // end of hyperscale namespace
