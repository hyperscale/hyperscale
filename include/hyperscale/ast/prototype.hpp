/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <string>
#include <vector>
#include <hyperscale/ast/expression.hpp>

namespace hyperscale {
namespace ast {

    /// Prototype - This class represents the "prototype" for a function,
    /// which captures its name, and its argument names (thus implicitly the number
    /// of arguments the function takes).
    class Prototype {
        std::string m_name;
        std::vector<std::string> m_args;

    public:
        Prototype(const std::string &name, std::vector<std::string> args)
        : m_name(name), m_args(std::move(args)) {}

        const std::string &getName() const {
            return m_name;
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
