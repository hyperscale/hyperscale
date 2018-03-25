/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <vector>
#include <memory>

namespace hyperscale {
namespace ast {

    class Node;

    class NodeRoot {
    private:
        std::vector<Node*> m_top_level_decls;

    public:
        NodeRoot() {}
        ~NodeRoot() {}

        void addNode(Node* node) {
            m_top_level_decls.push_back(node);
        }

        std::vector<Node*> getNodes() const {
            return m_top_level_decls;
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
