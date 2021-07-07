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

#include "lib/hyperscale/ast/node.hpp"

namespace hyperscale {
namespace ast {

    class SourceFile: public Node
    {
    protected:
        std::vector<Node*> m_decls;

    public:
        SourceFile();

        SourceFile(std::vector<Node*> decls);

        SourceFile(const SourceFile&) = delete;

        SourceFile& operator=(const SourceFile&) = delete;

        ~SourceFile();

        void addNode(Node* node);

        std::vector<Node*> getDecls();

        void accept(Visitor& visitor) override;
    };

} // end of ast namespace
} // end of hyperscale namespace
