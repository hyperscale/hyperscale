/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/source-file.hpp>

namespace hyperscale {
namespace ast {

    SourceFile::SourceFile(): Node() {}

    SourceFile::SourceFile(std::vector<Node*> decls):
        Node(),
        m_decls(decls) {}

    SourceFile::~SourceFile() {
        for (auto decl = m_decls.begin(); decl != m_decls.end(); ++decl) {
            delete (*decl);
        }

        m_decls.clear();
    }

    void SourceFile::addNode(Node* node) {
        m_decls.push_back(node);
    }

    std::vector<Node*> SourceFile::getDecls() {
        return m_decls;
    }

    void SourceFile::accept(Visitor& visitor) {
        visitor(*this);
    }

} // end of ast namespace
} // end of hyperscale namespace



