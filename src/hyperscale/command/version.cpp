/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/command/version.hpp>
#include <hyperscale/hyperscale.hpp>

namespace hyperscale {
namespace command {

    namespace po = boost::program_options;

    int version(po::variables_map &vm, po::parsed_options &options, po::options_description &desc) {
        hyperscale::show_version();
        hyperscale::show_copyright();
        hyperscale::show_llvm_version();

        return EXIT_SUCCESS;
    }

    std::shared_ptr<console::Command> versionCommand() {
        auto cmd = std::make_shared<console::Command>();

        cmd->name("version");
        cmd->description("Show the hyperscale version information");
        cmd->handle([]() {
            hyperscale::show_version();
            hyperscale::show_copyright();
            hyperscale::show_llvm_version();
        });

        return cmd;
    }

} // end of command namespace
} // end of hyperscale namespace
