/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/console/command.hpp>
#include <memory>

namespace hyperscale {
namespace command {
namespace debug {

    std::shared_ptr<console::Command> lexerCommand();

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace