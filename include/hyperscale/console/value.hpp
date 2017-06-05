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
#include <sstream>
#include <string>
#include <vector>

namespace hyperscale {
namespace console {

    template<class T>
    class Value : public Option {
    public:
        Value(const std::string& shortOption, const std::string& longOption, const std::string& description);
        Value(const std::string& shortOption, const std::string& longOption, const std::string& description, const T& defaultVal);
        Value(const std::string& shortOption, const std::string& longOption, const std::string& description, const T& defaultVal, T* assignTo);

        Value<T>& assignTo(T* var);

        Value<T>& setDefault(const T& value);

        T getValue(std::size_t idx = 0) const;

    protected:
        virtual void parse(const std::string& whatOption, const char* value);

        virtual std::string optionToString() const;

        virtual OptionValue getType() const;

        virtual void addValue(const T& value);

        virtual void updateReference();

        T* m_assign_to;
        std::vector<T> m_values;
        T m_default;
        bool m_has_default;
    };

    template<class T>
    Value<T>::Value(const std::string& shortOption, const std::string& longOption, const std::string& description) :
        Option(shortOption, longOption, description),
        m_assign_to(NULL),
        m_has_default(false)
    {
    }

    template<class T>
    Value<T>::Value(const std::string& shortOption, const std::string& longOption, const std::string& description, const T& defaultVal) :
        Option(shortOption, longOption, description),
        m_default(defaultVal),
        m_has_default(true)
    {
    }


    template<class T>
    Value<T>::Value(const std::string& shortOption, const std::string& longOption, const std::string& description, const T& defaultVal, T* assignTo) :
        Option(shortOption, longOption, description),
        m_assign_to(assignTo),
        m_default(defaultVal),
        m_has_default(true)
    {
        updateReference();
    }

    template<class T>
    std::string Value<T>::optionToString() const {
        std::stringstream ss;
        ss << Option::optionToString() << " arg";

        if (m_has_default) {
            std::stringstream defaultStr;
            defaultStr << m_default;
            if (!defaultStr.str().empty()) {
                ss << " (=" << m_default << ")";
            }
        }

        return ss.str();
    }

    template<class T>
    Value<T>& Value<T>::assignTo(T* var) {
        m_assign_to = var;

        return *this;
    }


    template<class T>
    Value<T>& Value<T>::setDefault(const T& value) {
        m_default = value;
        m_has_default = true;

        return *this;
    }


    template<class T>
    void Value<T>::updateReference() {
        if (m_assign_to != NULL) {
            if (isSet() || m_has_default) {
                *m_assign_to = getValue();
            }
        }
    }


    template<class T>
    void Value<T>::addValue(const T& value) {
        m_values.push_back(value);
        ++m_count;

        updateReference();
    }


    template<class T>
    T Value<T>::getValue(size_t idx) const {
        if (!isSet()) {
            if (m_has_default) {
                return m_default;
            } else {
                std::stringstream optionStr;

                if (getShortOption() != 0) {
                    optionStr << "-" << getShortOption();
                } else {
                    optionStr << "--" << getLongOption();
                }

                throw std::out_of_range("option not set: \"" + optionStr.str() + "\"");
            }
        }

        if (idx >= m_count) {
            std::stringstream optionStr;
            optionStr << "index out of range (" << idx << ") for \"";

            if (getShortOption() != 0) {
                optionStr << "-" << getShortOption();
            } else {
                optionStr << "--" << getLongOption();
            }

            optionStr << "\"";

            throw std::out_of_range(optionStr.str());
        }

        return m_values[idx];
    }


    template<class T>
    OptionValue Value<T>::getType() const {
        return OptionValue::Required;
    }

    template<>
    void Value<std::string>::parse(const std::string& whatOption, const char* value) {
        if (strlen(value) == 0) {
            throw std::invalid_argument("missing argument for " + whatOption);
        }

        addValue(value);
    }


    template<class T>
    void Value<T>::parse(const std::string& whatOption, const char* value) {
        T parsedValue;
        std::string strValue;

        if (value != NULL) {
            strValue = value;
        }

        std::istringstream is(strValue);
        int valuesRead = 0;

        while (is.good()) {
            if (is.peek() != EOF) {
                is >> parsedValue;
            } else {
                break;
            }

            valuesRead++;
        }

        if (is.fail()) {
            throw std::invalid_argument("invalid argument for " + whatOption + ": '" + strValue + "'");
        }

        if (valuesRead > 1) {
            throw std::invalid_argument("too many arguments for " + whatOption + ": '" + strValue + "'");
        }

        if (strValue.empty()) {
            throw std::invalid_argument("missing argument for " + whatOption);
        }

        addValue(parsedValue);
    }

} // end of console namespace
} // end of hyperscale namespace
