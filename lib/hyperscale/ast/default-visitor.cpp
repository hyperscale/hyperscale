/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "lib/hyperscale/ast/default-visitor.hpp"

namespace hyperscale {
namespace ast {
    DefaultVisitor::DefaultVisitor(): Visitor() {}

    DefaultVisitor::~DefaultVisitor() {}

    void DefaultVisitor::operator()(SourceFile& e) {}

    void DefaultVisitor::operator()(VarDecl& e) {}

    void DefaultVisitor::operator()(OpExpr& e) {}

    void DefaultVisitor::operator()(IntExpr& e) {}

    void DefaultVisitor::operator()(Node&) {}

    void DefaultVisitor::operator()(ParenExpr&) {}

    void DefaultVisitor::operator()(CallExpr&) {}

    void DefaultVisitor::operator()(DeclRefExpr&) {}

} // end of ast namespace
} // end of hyperscale namespace



