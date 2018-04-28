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

    class PrettyPrinterVisitor: virtual public DefaultVisitor {
    protected:
        /// The stream to print on.
        std::ostream& m_ostr;

    public:
        using super_type = DefaultVisitor;
        // Import overloaded virtual functions.
        using super_type::operator();

        /// Build to print on \a ostr.
        PrettyPrinterVisitor(std::ostream& ostr);

        void operator()(VarDecl& e);

        void operator()(OpExpr& e);

        void operator()(IntExpr& e);
    };

    std::ostream& operator<<(std::ostream& os, Node& tree);

} // end of ast namespace
} // end of hyperscale namespace


