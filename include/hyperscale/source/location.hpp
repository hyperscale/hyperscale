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

    class manager;

    class location
    {
    private:
        llvm::SMLoc m_value;

    public:
        location() {}
        explicit location(llvm::SMLoc value): m_value(value) {}

        ~location() {}

        bool isValid() const {
            return m_value.isValid();
        }

        bool isInvalid() const {
            return !m_value.isValid();
        }

        bool operator==(const location &RHS) const {
            return RHS.getValue() == m_value;
        }

        bool operator!=(const location &RHS) const {
            return RHS.getValue() != m_value;
        }

        llvm::SMLoc& getValue() {
            return m_value;
        }

        /// Return a source location advanced a specified number of bytes.
        location getAdvancedLoc(int byte_offset) const {
            assert(isValid() && "Can't advance an invalid location");

            return location(llvm::SMLoc::getFromPointer(m_value.getPointer() + byte_offset));
        }

        location getAdvancedLocOrInvalid(int byte_offset) const {
            if (isValid()) {
                return getAdvancedLoc(byte_offset);
            }

            return location();
        }

        const void* getOpaquePointerValue() const {
            return m_value.getPointer();
        }

  /// Print out the SourceLoc.  If this location is in the same buffer
  /// as specified by \c LastBufferID, then we don't print the filename.  If
  /// not, we do print the filename, and then update \c LastBufferID with the
  /// BufferID printed.
        void print(raw_ostream& OS, const manager& SM, unsigned& LastBufferID) const;

        void printLineAndColumn(raw_ostream& OS, const manager& SM) const;

        void print(raw_ostream& OS, const manager& SM) const {
            unsigned Tmp = ~0U;
            print(OS, SM, Tmp);
        }

        void dump(const manager& SM) const;
    };

} // end of source namespace
} // end of hyperscale namespace
