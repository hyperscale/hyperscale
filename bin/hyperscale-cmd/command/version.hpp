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

namespace hyperscale {
namespace command {

    class VersionCommand: public hyper::console::Command {
    public:
        void configuration();

        int execute();
    };

} // end of command namespace
} // end of hyperscale namespace
