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
#include <hyperscale/ast/node.hpp>

namespace hyperscale {
namespace ast {

    class FileSource: public Node
    {
    protected:
        std::vector<Node*> m_decls;

    public:
        FileSource();

        FileSource(std::vector<Node*> decls);

        FileSource(const FileSource&) = delete;

        FileSource& operator=(const FileSource&) = delete;

        ~FileSource();

        void addNode(Node* node);

        std::vector<Node*> getDecls();

        void accept(Visitor& visitor) override;
    };

} // end of ast namespace
} // end of hyperscale namespace
