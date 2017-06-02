/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <ostream>
#include <string>

namespace hyperscale {
namespace parser {

    struct Token {
        int type;
        std::string value;
        int start;
        int end;
        std::size_t length;
    };

    std::ostream& operator<<(std::ostream& os, const Token& token);


} // end of parser namespace
} // end of hyperscale namespace
