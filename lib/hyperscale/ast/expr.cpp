/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/ast/expr.hpp>


namespace hyperscale {
namespace ast {

    Expr::Expr(): Node() {}

    Expr::Expr(parser::Token& token): Node(token) {}

} // end of ast namespace
} // end of hyperscale namespace



