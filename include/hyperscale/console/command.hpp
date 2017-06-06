/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <functional>
#include <hyperscale/console/option.hpp>
#include <hyperscale/console/option_value.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace hyperscale {
namespace console {

    class Command;

    class Command: public std::enable_shared_from_this<Command> {
    protected:
        std::string m_name;

        std::string m_description;

        std::vector<std::shared_ptr<Option>> m_options;

        std::map<std::string, std::shared_ptr<Command>> m_commands;

        std::vector<std::string> m_args;

        std::shared_ptr<Command> m_parent;

        std::function<int(Command&)> m_handle;

    public:
        Command();

        Command& parent(std::shared_ptr<Command> command);

        Command& name(const std::string& name);

        std::string getName() const;

        Command& description(const std::string& description);

        std::string getDescription() const;

        Command& option(std::shared_ptr<Option> option);

        Command& command(std::shared_ptr<Command> command);

        bool hasCommand() const;

        Command& handle(std::function<int(Command&)> handle);

        std::shared_ptr<Option> getLongOpt(const std::string& opt) const;

        std::shared_ptr<Option> getShortOpt(char opt) const;

        Command& parse(int argc, char *argv[]);

        int run();

        std::string getNameWithParent() const;

        std::string help() const;

        std::vector<std::string> descriptionToString(std::size_t width = 40) const;
    };

} // end of console namespace
} // end of hyperscale namespace
