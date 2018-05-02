/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/default-visitor.hpp>

namespace hyperscale {
namespace ast {

    class GraphVisitor: virtual public DefaultVisitor {
    protected:
        /// The stream to print on.
        std::ostream& m_ostr;

    public:
        using super_type = DefaultVisitor;
        // Import overloaded virtual functions.
        using super_type::operator();

        /// Build to print on \a ostr.
        GraphVisitor(std::ostream& ostr);

        ~GraphVisitor();

        void operator()(SourceFile& e);

        void operator()(VarDecl& e);

        void operator()(Node&);

        void operator()(OpExpr& e);

        void operator()(IntExpr& e);

        void operator()(ParenExpr& e);

        void operator()(CallExpr& e);

        void operator()(DeclRefExpr& e);
    };

} // end of ast namespace
} // end of hyperscale namespace


