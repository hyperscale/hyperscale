/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <boost/program_options.hpp>

namespace hyperscale {
namespace command {
namespace debug {

    namespace po = boost::program_options;

    int lexer(po::variables_map &vm, po::parsed_options &options, po::options_description &desc);

} // end of debug namespace
} // end of command namespace
} // end of hyperscale namespace
