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
#include <hyperscale/ast/node_binary_operator_expression.hpp>
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

        void initDataFromType() {
            switch (m_type) {
                case NodeTypeRoot:
                    data = std::make_shared<NodeRoot>();
                    break;
                case NodeTypeVariableDeclaration:
                    data = std::make_shared<NodeVariableDeclaration>();
                    break;
                case NodeTypeGroupedExpr:
                    data = std::make_shared<NodeBinaryOperatorExpression>();
                default:
                    break;

            }
        }

    public:
        std::variant<
            std::shared_ptr<NodeRoot>,
            std::shared_ptr<Node>,
            std::shared_ptr<NodeVariableDeclaration>,
            std::shared_ptr<NodeBinaryOperatorExpression>
        > data;

        Node():
            m_type(NodeTypeRoot),
            m_line(0),
            m_column(0)
        {
            initDataFromType();
        }

        Node(NodeType type):
            m_type(type),
            m_line(0),
            m_column(0)
        {
            initDataFromType();
        }

        Node(NodeType type, hyperscale::parser::Token token):
            m_type(type),
            m_line(token.getLine()),
            m_column(token.getColumn())
        {
            initDataFromType();
        }

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

        template<typename T>
        std::shared_ptr<T> getData() {
            return std::get<std::shared_ptr<T>>(data);
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
