/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/parser/token.hpp>
#include <istream>
#include <string>
#include <vector>

namespace hyperscale {
namespace parser {

    class Lexer {
        public:
        Lexer();

        std::vector<Token> lex(std::istream &source);
    };



} // end of parser namespace
} // end of hyperscale namespace
