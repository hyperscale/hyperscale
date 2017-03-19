/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

namespace hyperscale {
namespace v1 {

    typedef enum TOKEN {
        token_eof = -1,

        // commands
        token_def = -2,
        token_extern = -3,

        // primary
        token_identifier = -4,
        token_number = -5,

        // control
        token_if = -6,
        token_then = -7,
        token_else = -8,
        token_for = -9,
        token_in = -10,

        // operators
        token_binary = -11,
        token_unary = -12,

        // var definition
        token_var = -13
    } token_t;

    std::string to_string(token_t tok) {
        switch (tok) {
        case token_eof:
            return "eof";
        case token_def:
            return "def";
        case token_extern:
            return "extern";
        case token_identifier:
            return "identifier";
        case token_number:
            return "number";
        case token_if:
            return "if";
        case token_then:
            return "then";
        case token_else:
            return "else";
        case token_for:
            return "for";
        case token_in:
            return "in";
        case token_binary:
            return "binary";
        case token_unary:
            return "unary";
        case token_var:
            return "var";
        }

        return std::string(1, (char)Tok);
    }

} // end of v1 namespace
} // end of hyperscale namespace
