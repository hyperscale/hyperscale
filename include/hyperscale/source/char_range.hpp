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
#include <hyperscale/source/location.hpp>
#include <hyperscale/source/range.hpp>

namespace hyperscale {
namespace source {

    /// A half-open character-based source range.
    class char_range {
    private:
        location m_start;
        unsigned m_byte_length;

    public:
        /// \brief Constructs an invalid range.
        char_range() {}

        char_range(location start, unsigned byte_length): m_start(start), m_byte_length(byte_length) { }

        /// \brief Constructs a character range which starts and ends at the
        /// specified character locations.
        char_range(const manager& SM, location start, location end);

        /// Use Lexer::getCharSourceRangeFromSourceRange() instead.
        char_range(const manager& SM, range range_) = delete;

        bool isValid() const {
            return m_start.isValid();
        }

        bool isInvalid() const {
            return m_start.isInvalid();
        }

        location getStart() const {
            return m_start;
        }

        location getEnd() const {
            if (m_start.isValid()) {
                return m_start.getAdvancedLoc(m_byte_length);
            } else {
                return location();
            }
        }

        /// Returns true if the given source location is contained in the range.
        bool contains(location loc) const {
            auto less = std::less<const char *>();
            auto less_equal = std::less_equal<const char *>();
            return (
                less_equal(getStart().getValue().getPointer(), loc.getValue().getPointer()) &&
                less(loc.getValue().getPointer(), getEnd().getValue().getPointer())
            );
        }

        bool contains(char_range other) const {
            auto less_equal = std::less_equal<const char *>();
            return (
                contains(other.getStart()) &&
                less_equal(other.getEnd().getValue().getPointer(), getEnd().getValue().getPointer())
            );
        }

        /// \brief expands *this to cover Other
        void widen(char_range other) {
            auto diff = other.getEnd().getValue().getPointer() - getEnd().getValue().getPointer();
            if (diff > 0) {
                m_byte_length += diff;
            }

            const auto my_start_ptr = getStart().getValue().getPointer();
            diff = my_start_ptr - other.getStart().getValue().getPointer();
            if (diff > 0) {
                m_byte_length += diff;
                m_start = location(llvm::SMLoc::getFromPointer(my_start_ptr - diff));
            }
        }

        bool overlaps(char_range other) const {
            return contains(other.getStart()) || contains(other.getEnd());
        }

        StringRef str() const {
            return StringRef(m_start.getValue().getPointer(), m_byte_length);
        }

        /// \brief Return the length of this valid range in bytes.  Can be zero.
        unsigned getByteLength() const {
            assert(isValid() && "length does not make sense for an invalid range");

            return m_byte_length;
        }

        /// Print out the char_range.  If the locations are in the same buffer
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
