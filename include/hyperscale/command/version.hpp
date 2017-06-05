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
#include <hyperscale/console/command.hpp>
#include <memory>

namespace hyperscale {
namespace command {

    namespace po = boost::program_options;

    int version(po::variables_map &vm, po::parsed_options &options, po::options_description &desc);

    std::shared_ptr<console::Command> versionCommand();

} // end of command namespace
} // end of hyperscale namespace
