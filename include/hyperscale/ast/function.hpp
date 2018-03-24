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
#include <hyperscale/ast/prototype.hpp>

namespace hyperscale {
namespace ast {

    /// Function - This class represents a function definition itself.
    class Function {
        std::unique_ptr<Prototype> m_prototype;
        std::unique_ptr<Expression> m_body;

    public:
        Function(std::unique_ptr<Prototype> prototype, std::unique_ptr<Expression> body)
            : m_prototype(std::move(prototype)), m_body(std::move(body)) {}
    };

} // end of ast namespace
} // end of hyperscale namespace
