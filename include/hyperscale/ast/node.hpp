/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <memory>
#include <hyperscale/ast/fwd.hpp>
#include <hyperscale/ast/visitor.hpp>
#include <hyperscale/ast/visitable.hpp>
#include <hyperscale/parser/token.hpp>

namespace hyperscale {
namespace ast {

    class Node: public Visitable<Node> {
    private:
        std::size_t m_line;
        std::size_t m_column;

    public:
        Node();
        Node(parser::Token& token);

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;

        virtual ~Node() = default;

        inline std::size_t getLine() const;

        inline std::size_t getColumn() const;
    };

} // end of ast namespace
} // end of hyperscale namespace

#include <hyperscale/ast/node.hxx>
