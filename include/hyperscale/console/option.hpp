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
#include <hyperscale/console/option_value.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace hyperscale {
namespace console {

    class Option {
        friend class Command;
    protected:
        std::string m_short_option;

        std::string m_long_option;

        std::string m_description;

        unsigned int m_count;

    public:
        Option(const std::string& shortOption, const std::string& longOption, const std::string& description) :
            m_short_option(shortOption),
            m_long_option(longOption),
            m_description(description),
            m_count(0)
        {
            if (shortOption.size() > 1) {
                throw std::invalid_argument("length of short option must be <= 1: '" + shortOption + "'");
            }

            if (shortOption.empty() && longOption.empty()) {
                throw std::invalid_argument("short and long option are empty");
            }
        }


        virtual ~Option() {}

        inline char getShortOption() const {
            if (!m_short_option.empty()) {
                return m_short_option[0];
            }

            return 0;
        }

        inline std::string getLongOption() const {
            return m_long_option;
        }

        inline std::string getDescription() const {
            return m_description;
        }

        inline unsigned int count() const {
            return m_count;
        }

        inline bool isSet() const {
            return (count() > 0);
        }

    protected:
        virtual void parse(const std::string& whatOption, const char* value) = 0;

        virtual void updateReference() {
        }

        virtual std::string optionToString() const {
            std::stringstream line;

            if (getShortOption() != 0) {
                line << "  -" << getShortOption();
                if (!getLongOption().empty()) {
                    line << ", ";
                }
            } else {
                line << "  ";
            }

            if (!getLongOption().empty()) {
                line << "--" << getLongOption();
            }

            return line.str();
        }

        virtual std::vector<std::string> descriptionToString(std::size_t width = 40) const {
            std::vector<std::string> lines;
            std::stringstream description(getDescription());
            std::string line;

            while (std::getline(description, line, '\n')) {
                lines.push_back(line);
            }

            return lines;
        }

        virtual OptionValue getType() const = 0;
    };

} // end of console namespace
} // end of hyperscale namespace
