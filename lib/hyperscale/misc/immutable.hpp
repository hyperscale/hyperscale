/**
 * Hyperscale
 *
 * (c) 2015-2018 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

namespace hyperscale {
namespace misc {

    template <typename T>
    struct Immutable {
        using type = const T;
    };

} // end of misc namespace
} // end of hyperscale namespace

