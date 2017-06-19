/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyper/console/command.hpp>
#include <command/debug/lexer.hpp>

namespace hyperscale {
namespace command {

    class DebugCommand: public hyper::console::Command {
    public:
        void configuration();
    };

} // end of command namespace
} // end of hyperscale namespace
