/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/console/option.hpp>
#include <hyperscale/console/option_value.hpp>
#include <hyperscale/console/value.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace hyperscale {
namespace console {

    class Switch : public Value<bool> {
    public:
        Switch(const std::string& shortOption, const std::string& longOption, const std::string& description);
        Switch(const std::string& shortOption, const std::string& longOption, const std::string& description, bool* assignTo);

    protected:
        virtual void parse(const std::string& whatOption, const char* value);

        virtual std::string optionToString() const;

        virtual OptionValue getType() const;

        Switch& setDefault(const bool& value);
    };

    Switch::Switch(const std::string& shortOption, const std::string& longOption, const std::string& description) :
        Value<bool>(shortOption, longOption, description, false)
    {
    }


    Switch::Switch(const std::string& shortOption, const std::string& longOption, const std::string& description, bool* assignTo) :
        Value<bool>(shortOption, longOption, description, false, assignTo)
    {
    }


    void Switch::parse(const std::string& whatOption, const char* value) {
        addValue(true);
    }


    OptionValue Switch::getType() const {
        return OptionValue::None;
    }


    std::string Switch::optionToString() const {
        return Option::optionToString();
    }

} // end of console namespace
} // end of hyperscale namespace
