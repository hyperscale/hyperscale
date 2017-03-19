
/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/SMLoc.h>
#include <functional>
#include <hyperscale/source/manager.hpp>

namespace hyperscale {
namespace source {



    /// range in swift is a pair of locations.  However, note that the end
    /// location is the start of the last token in the range, not the last character
    /// in the range.  This is unlike SMRange, so we use a distinct type to make
    /// sure that proper conversions happen where important.
    class range {
    public:
        location start;
        location end;

        range() {}
        range(location loc) : start(loc), end(loc) { }
        range(location start_, location end_) : start(start_), end(end_) {
            assert(start.isValid() == end.isValid() && "Start and end should either both be valid or both be invalid!");
        }

        bool isValid() const {
            return start.isValid();
        }

        bool isInvalid() const {
            return start.isInvalid();
        }

        /// Print out the range.  If the locations are in the same buffer
        /// as specified by LastBufferID, then we don't print the filename.  If not,
        /// we do print the filename, and then update LastBufferID with the BufferID
        /// printed.
        void print(raw_ostream &OS, const manager& SM, unsigned& last_buffer_id, bool print_text = true) const;

        void print(raw_ostream &OS, const manager& SM, bool print_text = true) const {
            unsigned Tmp = ~0U;
            print(OS, SM, Tmp, print_text);
        }

        void dump(const manager& SM) const;
    };



} // end of source namespace
} // end of hyperscale namespace
