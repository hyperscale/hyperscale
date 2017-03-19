/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/command/help.hpp>
#include <hyperscale/config.hpp>
#include <iostream>

namespace hyperscale {
namespace command {

    namespace po = boost::program_options;

    int help(po::variables_map &vm, po::parsed_options &option, po::options_description &desc) {
        std::cout << HYPERSCALE_NAME << " Compiler" << std::endl << std::endl << desc << std::endl << std::endl;

        return EXIT_SUCCESS;
    }

} // end of command namespace
} // end of hyperscale namespace
