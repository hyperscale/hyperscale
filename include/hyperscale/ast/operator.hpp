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
#include <ostream>

namespace hyperscale {
namespace ast {

    /// Operator qualifier.
    enum class Operator {
        // Arithmetics.
        /** \brief "+" */ add = 0,
        /** \brief "-" */ sub,
        /** \brief "*" */ mul,
        /** \brief "/" */ div,

        // Comparison.
        /** \brief "=" */  eq,
        /** \brief "<>" */ ne,
        /** \brief "<" */  lt,
        /** \brief "<=" */ le,
        /** \brief ">" */  gt,
        /** \brief ">=" */ ge
    };

    static const char* OperatorNames[] = {
        // Arithmetics.
        "+", "-", "*", "/",
        // Comparison.
        "=", "<>", "<", "<=", ">", ">="
    };

    std::ostream& operator<<(std::ostream& os, Operator op);

} // end of ast namespace
} // end of hyperscale namespace

