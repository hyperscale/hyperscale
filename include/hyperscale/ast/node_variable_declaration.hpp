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
#include <memory>

namespace hyperscale {
namespace ast {

    class Node;

    class NodeVariableDeclaration {
    private:
        bool m_is_const;

        std::string m_symbol;

        std::string m_type;

        Node* m_expression;

    public:
        NodeVariableDeclaration(): m_is_const(false), m_symbol(""), m_type("") {}
        ~NodeVariableDeclaration() {}

        void setConst(bool is_const) {
            m_is_const = is_const;
        }

        bool isConst() const {
            return m_is_const;
        }

        void setSymbol(std::string symbol) {
            m_symbol = symbol;
        }

        std::string getSymbol() const {
            return m_symbol;
        }

        void setType(std::string type) {
            m_type = type;
        }

        std::string getType() const {
            return m_type;
        }

        void setExpression(Node* expression) {
            m_expression = expression;
        }

        Node* getExpression() const {
            return m_expression;
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
