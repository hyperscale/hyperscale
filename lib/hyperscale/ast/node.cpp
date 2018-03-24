/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/node.hpp>


namespace hyperscale {
namespace ast {

    Node::Node():
        m_line(0),
        m_column(0) {}

    Node::Node(std::shared_ptr<parser::Token>& token):
        m_line(token->getLine()),
        m_column(token->getColumn()) {}

} // end of ast namespace
} // end of hyperscale namespace



