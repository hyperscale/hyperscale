/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/ast/visitor.hpp>

namespace hyperscale {
namespace ast {

    template <template <typename> class Const>
    class GenericDefaultVisitor : public virtual GenericVisitor<Const> {
    public:
        /// Super class type.
        using super_type = GenericVisitor<Const>;

        // Import overloaded \c operator() methods.
        using super_type::operator();

        /// Convenient abbreviation.
        template <typename Type>
        using const_t = typename Const<Type>::type;

        /// Construct a default visitor.
        GenericDefaultVisitor();

        /// Destroy a default visitor.
        virtual ~GenericDefaultVisitor();

        void operator()(const_t<VarDec>& e) override {

        }
    };

} // end of ast namespace
} // end of hyperscale namespace


