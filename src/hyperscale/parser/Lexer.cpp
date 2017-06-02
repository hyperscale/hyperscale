/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <hyperscale/parser/lexer.hpp>
#include <hyperscale/parser/token.hpp>
#include <iostream>

namespace hyperscale {
namespace parser {

    Lexer::Lexer() {

    }

    std::vector<Token> Lexer::lex(std::istream &source) {
        char c;
        while (source.get(c)) {
            std::cout << c;
        }

        return std::vector<Token>();
    }

} // end of parser namespace
} // end of hyperscale namespace
