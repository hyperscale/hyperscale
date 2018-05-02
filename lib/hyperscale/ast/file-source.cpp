/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/file-source.hpp>

namespace hyperscale {
namespace ast {

    FileSource::FileSource(): Node() {}

    FileSource::FileSource(std::vector<Node*> decls):
        Node(),
        m_decls(decls) {}

    FileSource::~FileSource() {
        for (auto decl = m_decls.begin(); decl != m_decls.end(); ++decl) {
            delete (*decl);
        }

        m_decls.clear();
    }

    void FileSource::addNode(Node* node) {
        m_decls.push_back(node);
    }

    std::vector<Node*> FileSource::getDecls() {
        return m_decls;
    }
/*
    void FileSource::accept(Visitor& visitor) {
        visitor(*this);
    }
*/
} // end of ast namespace
} // end of hyperscale namespace



