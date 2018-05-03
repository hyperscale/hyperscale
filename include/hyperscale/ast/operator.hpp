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

    static const std::map<std::string, Operator> OperatorNameToOperatorMap = {
        // Arithmetics.
        {"+", Operator::add},
        {"-", Operator::sub},
        {"*", Operator::mul},
        {"/", Operator::div},
        // Comparison.
        {"=", Operator::eq},
        {"<>", Operator::ne},
        {"<", Operator::lt},
        {"<=", Operator::le},
        {">", Operator::gt},
        {">=", Operator::ge},
    };

    static const std::map<Operator, std::string> OperatorToOperatorNameMap = {
        // Arithmetics.
        {Operator::add, "+"},
        {Operator::sub, "-"},
        {Operator::mul, "*"},
        {Operator::div, "/"},
        // Comparison.
        {Operator::eq, "="},
        {Operator::ne, "<>"},
        {Operator::lt, "<"},
        {Operator::le, "<="},
        {Operator::gt, ">"},
        {Operator::ge, ">="},
    };

    std::ostream& operator<<(std::ostream& os, Operator op);

} // end of ast namespace
} // end of hyperscale namespace

