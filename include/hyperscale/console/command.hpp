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
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace hyperscale {
namespace console {

    class Command;

    class Command/*: public std::enable_shared_from_this<Command>*/ {
    protected:
        std::string m_name;

        std::string m_description;

        std::vector<std::shared_ptr<Option>> m_options;

        std::map<std::string, std::shared_ptr<Command>> m_commands;

        std::vector<std::string> m_args;

        std::shared_ptr<Command> m_parent;

        std::function<int(const Command& cmd)> m_handle;

        std::vector<std::string> m_argv;

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

        Command& handle(const std::function<int(const Command& cmd_)> handle_);

        std::shared_ptr<Option> getLongOpt(const std::string& opt) const;

        std::shared_ptr<Option> getShortOpt(const char opt) const;

        Command& parse(int argc, char *argv[]);

        Command& parse(std::vector<std::string>& args);

        Command& setOptions(const std::vector<std::shared_ptr<Option>>& options);

        int run();

        int exec();

        std::string getNameWithParent() const;

        std::string help() const;

        std::vector<std::string> descriptionToString(std::size_t width = 40) const;
    };

} // end of console namespace
} // end of hyperscale namespace
