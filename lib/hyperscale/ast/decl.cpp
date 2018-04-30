/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/decl.hpp>

namespace hyperscale {
namespace ast {

    Decl::Decl(): Node() {}

    Decl::Decl(parser::Token& token): Node(token) {}

} // end of ast namespace
} // end of hyperscale namespace
