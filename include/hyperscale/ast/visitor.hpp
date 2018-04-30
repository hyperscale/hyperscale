/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/node.hpp>

namespace hyperscale {
namespace ast {

    class OpExpr;
    class IntExpr;
    class Node;
    class ParenExpr;

    class Visitor {
    public:
        virtual void operator()(Node&) = 0;
        virtual void operator()(OpExpr&) = 0;
        virtual void operator()(IntExpr&) = 0;
        virtual void operator()(ParenExpr&) = 0;
        // virtual void operator()(VarDecl&) = 0;
    };

} // end of ast namespace
} // end of hyperscale namespace

// #include <hyperscale/ast/visitor.hxx>
