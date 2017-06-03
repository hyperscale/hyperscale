/**
 * Hyperscale
 *
 * (c) 2015-2017 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <llvm/ADT/StringRef.h>

namespace hyperscale {
namespace ast {

    class Identifier {
    public:
        /// isOperatorStartCodePoint - Return true if the specified code point is a
        /// valid start of an operator.
        static bool isOperatorStartCodePoint(uint32_t C) {
            // ASCII operator chars.
            static const char OpChars[] = "/=-+*%<>!&|^~.?";

            if (C < 0x80) {
                return memchr(OpChars, C, sizeof(OpChars) - 1) != 0;
            }

            // Unicode math, symbol, arrow, dingbat, and line/box drawing chars.
            return (C >= 0x00A1 && C <= 0x00A7)
                || C == 0x00A9 || C == 0x00AB || C == 0x00AC || C == 0x00AE
                || C == 0x00B0 || C == 0x00B1 || C == 0x00B6 || C == 0x00BB
                || C == 0x00BF || C == 0x00D7 || C == 0x00F7
                || C == 0x2016 || C == 0x2017 || (C >= 0x2020 && C <= 0x2027)
                || (C >= 0x2030 && C <= 0x203E) || (C >= 0x2041 && C <= 0x2053)
                || (C >= 0x2055 && C <= 0x205E) || (C >= 0x2190 && C <= 0x23FF)
                || (C >= 0x2500 && C <= 0x2775) || (C >= 0x2794 && C <= 0x2BFF)
                || (C >= 0x2E00 && C <= 0x2E7F) || (C >= 0x3001 && C <= 0x3003)
                || (C >= 0x3008 && C <= 0x3030);
        }

        /// isOperatorContinuationCodePoint - Return true if the specified code point
        /// is a valid operator code point.
        static bool isOperatorContinuationCodePoint(uint32_t C) {
            if (isOperatorStartCodePoint(C)) {
                return true;
            }

            // Unicode combining characters and variation selectors.
            return (C >= 0x0300 && C <= 0x036F)
                || (C >= 0x1DC0 && C <= 0x1DFF)
                || (C >= 0x20D0 && C <= 0x20FF)
                || (C >= 0xFE00 && C <= 0xFE0F)
                || (C >= 0xFE20 && C <= 0xFE2F)
                || (C >= 0xE0100 && C <= 0xE01EF);
        }

        static bool isEditorPlaceholder(llvm::StringRef name) {
            return name.startswith("<#");
        }
    };

} // end of ast namespace
} // end of hyperscale namespace
