/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <boost/program_options.hpp>

namespace hyperscale {
    namespace po = boost::program_options;

    typedef int (*command_t)(po::variables_map &vm, po::parsed_options &options, po::options_description &desc);

} // end of hyperscale namespace
