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
#include <hyperscale/console/implicit.hpp>
#include <hyperscale/console/switch.hpp>
#include <memory>

namespace hyperscale {
namespace console {

    class Application: public Command {
    public:
        Application(): Command() {
            option(std::make_shared<Switch>("h", "help", "Print usage"));
            option(std::shared_ptr<Implicit<std::string>>(new Implicit<std::string>(
                "l",
                "log-level",
                "Set the logging level (\"debug\", \"info\", \"warn\", \"error\", \"fatal\")",
                "info"
            )));
        }
    };

} // end of console namespace
} // end of hyperscale namespace
