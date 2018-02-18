/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <variant>
#include <memory>
#include <hyperscale/ast/node_root.hpp>
#include <hyperscale/ast/node_variable_declaration.hpp>
#include <hyperscale/parser/token.hpp>

namespace hyperscale {
namespace ast {

    class Node;
    class NodeRoot;
    class NodeVariableDeclaration;

    enum NodeType {
         NodeTypeRoot,
         NodeTypeIntLiteral,
         NodeTypeVariableDeclaration,
         NodeTypeGroupedExpr,
         NodeTypeVarLiteral,
         NodeTypeFnCallExpr
    };

    class Node {
    private:
        NodeType m_type;
        std::size_t m_line;
        std::size_t m_column;
    public:
        std::variant<
            std::shared_ptr<NodeRoot>,
            std::shared_ptr<Node>,
            std::shared_ptr<NodeVariableDeclaration>
        > data;

        Node():
            m_type(NodeTypeRoot),
            m_line(0),
            m_column(0) {}

        Node(NodeType type, hyperscale::parser::Token token):
            m_type(type),
            m_line(token.getLine()),
            m_column(token.getColumn()) {}

        Node(NodeType type, std::size_t line, std::size_t column):
            m_type(type),
            m_line(line),
            m_column(column) {}

        ~Node() {}

        NodeType getType() const {
            return m_type;
        }

        std::size_t getLine() const {
            return m_line;
        }

        std::size_t getColumn() const {
            return m_column;
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
