/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/console/command.hpp>
#include <hyperscale/console/option.hpp>
#include <iostream>
#include <memory>
#include <sstream>

namespace hyperscale {
namespace console {

    Command::Command() {
    }

    Command& Command::parent(std::shared_ptr<Command> command) {
        m_parent = std::move(command);

        return *this;
    }

    Command& Command::name(const std::string& name) {
        m_name = name;

        return *this;
    }

    Command& Command::description(const std::string& description) {
        m_description = description;

        return *this;
    }

    Command& Command::option(std::shared_ptr<Option> option) {
        for (std::size_t n = 0; n < m_options.size(); ++n) {
            if ((option->getShortOption() != 0) && (option->getShortOption() == m_options[n]->getShortOption())) {
                throw std::invalid_argument("dublicate short option '-" + std::string(1, option->getShortOption()) + "'");
            }

            if (!option->getLongOption().empty() && (option->getLongOption() == (m_options[n]->getLongOption()))) {
                throw std::invalid_argument("dublicate long option '--" + option->getLongOption() + "'");
            }
        }

        m_options.push_back(std::move(option));

        return *this;
    }

    Command& Command::command(std::shared_ptr<Command> cmd) {
        cmd->parent(std::shared_ptr<Command>(this));

        m_commands.push_back(std::move(cmd));

        return *this;
    }

    Command& Command::handle(std::function<void()> handle) {
        m_handle = handle;

        return *this;
    }

    std::shared_ptr<Option> Command::getLongOpt(const std::string& opt) const {
        for (std::size_t n = 0; n < m_options.size(); ++n) {
            if (m_options[n]->getLongOption() == opt) {
                return std::move(m_options[n]);
            }
        }

        return NULL;
    }


    std::shared_ptr<Option> Command::getShortOpt(char opt) const
    {
        for (std::size_t n = 0; n < m_options.size(); ++n) {
            if (m_options[n]->getShortOption() == opt) {
                return std::move(m_options[n]);
            }
        }

        return NULL;
    }

    void Command::parse(int argc, char *argv[]) {
        m_args.clear();

        for (int n = 1; n < argc; ++n) {
            const std::string arg(argv[n]);

            if (arg == "--") {
                ///from here on only non opt args
                for (int m = n + 1; m < argc; ++m) {
                    m_args.push_back(argv[m]);
                }

                break;
            } else if (arg.find("--") == 0) {
                /// long option arg
                std::string opt = arg.substr(2);
                std::string optarg;
                std::size_t equalIdx = opt.find('=');

                if (equalIdx != std::string::npos) {
                    optarg = opt.substr(equalIdx + 1);
                    opt.resize(equalIdx);
                }

                std::shared_ptr<Option> option = NULL;
                if ((option = getLongOpt(opt))) {
                    if (option->getType() == OptionValue::None) {
                        if (!optarg.empty()) {
                            option = NULL;
                        }
                    } else if (option->getType() == OptionValue::Required) {
                        if (optarg.empty() && n < argc-1) {
                            optarg = argv[++n];
                        }
                    }
                }

                if (option) {
                    option->parse(opt, optarg.c_str());
                } else {
                    //@TODO throw error
                    //m_unknown_options.push_back(arg);
                    std::cerr << "Unknown options: " << arg << std::endl;
                }
            } else if (arg.find("-") == 0) {
                /// short option arg
                std::string opt = arg.substr(1);
                bool unknown = false;

                for (std::size_t m = 0; m < opt.size(); ++m) {
                    char c = opt[m];
                    std::shared_ptr<Option> option = NULL;
                    std::string optarg;

                    if ((option = getShortOpt(c))) {
                        if (option->getType() == OptionValue::Required) {
                            /// use the rest of the current argument as optarg
                            optarg = opt.substr(m + 1);
                            /// or the next arg
                            if (optarg.empty() && n < argc-1) {
                                optarg = argv[++n];
                            }

                            m = opt.size();
                        } else if (option->getType() == OptionValue::Optional) {
                            /// use the rest of the current argument as optarg
                            optarg = opt.substr(m + 1);
                            m = opt.size();
                        }
                    }

                    if (option) {
                        option->parse(std::string(1, c), optarg.c_str());
                    } else {
                        unknown = true;
                    }
                }

                if (unknown) {
                    //@TODO throw error
                    std::cerr << "Unknown options: " << arg << std::endl;
                    //m_unknown_options.push_back(arg);
                }
            } else {
                m_args.push_back(arg);
            }
        }

    }

    std::string Command::getNameWithParent() const {
        std::stringstream s;

        if (m_parent) {
            s << m_parent->getNameWithParent() << " ";
        }

        s << m_name;

        return s.str();
    }

    std::string Command::help() const {

        std::size_t optionRightMargin(20);
        const std::size_t maxDescriptionLeftMargin(40);
    //	const std::size_t descriptionRightMargin(80);

        for (std::size_t opt = 0; opt < m_options.size(); ++opt) {
            optionRightMargin = std::max(optionRightMargin, m_options[opt]->optionToString().size() + 2);
        }

        optionRightMargin = std::min(maxDescriptionLeftMargin - 2, optionRightMargin);

        std::stringstream s;

        s << "Usage:	" << getNameWithParent();

        if (m_commands.empty() && m_options.size() > 0) {
            s << " [OPTIONS]";
        } else if (m_commands.size() > 0) {
            s << " COMMAND";
        }

        s << std::endl;
        s << std::endl;

        if (!m_description.empty()) {
            s << m_description << std::endl;
            s << std::endl;
        }

        if (m_options.size() > 0) {
            s << "Options:" << std::endl;

            for (std::size_t opt = 0; opt < m_options.size(); ++opt) {
                std::string optionStr = m_options[opt]->optionToString();

                if (optionStr.size() < optionRightMargin) {
                    optionStr.resize(optionRightMargin, ' ');
                } else {
                    optionStr += "\n" + std::string(optionRightMargin, ' ');
                }

                s << optionStr;

                std::vector<std::string> lines = m_options[opt]->descriptionToString(20);
                std::string empty(optionRightMargin, ' ');

                for (std::size_t n = 0; n < lines.size(); ++n) {
                    if (n > 0) {
                        s << std::endl << empty;
                    }

                    s << lines[n];
                }

                s << std::endl;
            }

            // foreach options
        }

        if (m_commands.size() > 0) {
            s << "Commands:" << std::endl;

            // foreach commands

            s << "Run '" << getNameWithParent() << " COMMAND --help' for more information on a command." << std::endl;
        }

        return s.str();
    }

} // end of syntax namespace
} // end of hyperscale namespace
