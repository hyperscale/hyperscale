/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#include "lib/hyperscale/hyperscale.hpp"

#include "build.hpp"

namespace hyperscale {
namespace command {

    void BuildCommand::configuration() {
        setName("build");
        setDescription("Build project");
    }


    int BuildCommand::execute() {
        return EXIT_SUCCESS;
    }

} // end of command namespace
} // end of hyperscale namespace
