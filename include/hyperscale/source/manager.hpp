/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/source/location.hpp>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/Support/SourceMgr.h>
#include <map>

namespace hyperscale {
namespace source {

    class location;

    class manager {
    private:
        llvm::SourceMgr m_source_manager;

        unsigned m_code_completion_buffer_id = 0U;
        unsigned m_code_completion_offset;

        /// \brief The buffer ID where a hashbang line #! is allowed.
        unsigned m_hashbang_buffer_id = 0U;

        /// Associates buffer identifiers to buffer IDs.
        llvm::StringMap<unsigned> m_buf_ident_id_map;

        // #line directive handling.
        struct virtual_file_t {
            CharSourceRange range;
            std::string name;
            int line_offset;
        };

        std::map<const char *, virtual_file_t> m_virtual_files;
        mutable std::pair<const char *, const virtual_file_t*> m_cached_virtual_file = {};

        const virtual_file_t *getVirtualFile(location loc) const;

        int getLineOffset(location loc) const {
            if (auto VFile = getVirtualFile(loc)) {
                return VFile->line_offset;
            } else {
                return 0;
            }
        }

    public:
        llvm::SourceMgr& getLLVMSourceMgr() {
            return m_source_manager;
        }

        const llvm::SourceMgr& getLLVMSourceMgr() const {
            return m_source_manager;
        }

        void setCodeCompletionPoint(unsigned buffer_id, unsigned offset) {
            assert(buffer_id != 0U && "Buffer should be valid");
            m_code_completion_buffer_id = buffer_id;
            m_code_completion_offset = offset;
        }

        unsigned getCodeCompletionBufferID() const {
            return m_code_completion_buffer_id;
        }

        unsigned getCodeCompletionOffset() const {
            return m_code_completion_offset;
        }

        location getCodeCompletionLoc() const;

        void setHashbangBufferID(unsigned buffer_id) {
            assert(m_hashbang_buffer_id == 0U && "Hashbang buffer ID already set");
            m_hashbang_buffer_id = buffer_id;
        }

        unsigned getHashbangBufferID() const {
            return m_hashbang_buffer_id;
        }

        /// Returns true if \c LHS is before \c RHS in the source buffer.
        bool isBeforeInBuffer(location LHS, location RHS) const {
            return LHS.getValue().getPointer() < RHS.getValue().getPointer();
        }

        /// Returns the buffer ID for the specified *valid* location.
        ///
        /// Because a valid source location always corresponds to a source buffer,
        /// this routine always returns a valid buffer ID.
        unsigned findBufferContainingLoc(location loc) const;

        /// Adds a memory buffer to the SourceManager, taking ownership of it.
        unsigned addNewSourceBuffer(std::unique_ptr<llvm::MemoryBuffer> buffer);

        /// Add a #line-defined virtual file region.
        ///
        /// By default, this region continues to the end of the buffer.
        ///
        /// \returns True if the new file was added, false if the file already exists.
        /// The name and line offset must match exactly in that case.
        ///
        /// \sa closeVirtualFile.
        bool openVirtualFile(location loc, StringRef name, int line_offset);

        /// Close a #line-defined virtual file region.
        void closeVirtualFile(location end);

        /// Creates a copy of a \c MemoryBuffer and adds it to the \c SourceManager,
        /// taking ownership of the copy.
        unsigned addMemBufferCopy(llvm::MemoryBuffer* buffer);

        /// Creates and adds a memory buffer to the \c SourceManager, taking
        /// ownership of the newly created copy.
        ///
        /// \p InputData and \p BufIdentifier are copied, so that this memory can go
        /// away as soon as this function returns.
        unsigned addMemBufferCopy(StringRef input_data, StringRef buf_identifier = "");

        /// Returns a buffer ID for a previously added buffer with the given
        /// buffer identifier, or None if there is no such buffer.
        Optional<unsigned> getIDForBufferIdentifier(StringRef buf_identifier);

        /// Returns the identifier for the buffer with the given ID.
        ///
        /// \p BufferID must be a valid buffer ID.
        const char* getIdentifierForBuffer(unsigned buffer_id) const;

        /// \brief Returns a SourceRange covering the entire specified buffer.
        ///
        /// Note that the start location might not point at the first token: it
        /// might point at whitespace or a comment.
        char_source_range getRangeForBuffer(unsigned buffer_id) const;

        /// Returns the location for the beginning of the specified buffer
        /// (at offset zero).
        ///
        /// Note that the resulting location might not point at the first token: it
        /// might point at whitespace or a comment.
        location getLocForBufferStart(unsigned buffer_id) const {
            return getRangeForBuffer(buffer_id).getStart();
        }

        /// \brief Returns the offset in bytes for the given valid source location.
        unsigned getLocOffsetInBuffer(location loc, unsigned buffer_id) const;

        /// \brief Returns the distance in bytes between the given valid source
        /// locations.
        unsigned getByteDistance(location Start, location End) const;

        /// Returns the location for the byte offset in the specified buffer.
        location getLocForOffset(unsigned buffer_id, unsigned offset) const {
            return getLocForBufferStart(buffer_id).getAdvancedLoc(offset);
        }

        /// Returns the identifier string for the buffer containing the given source
        /// location.
        ///
        /// This respects #line directives.
        const char* getBufferIdentifierForLoc(location loc) const {
            if (auto VFile = getVirtualFile(loc)) {
                return VFile->name.c_str();
            } else {
                return getIdentifierForBuffer(findBufferContainingLoc(loc));
            }
        }

        /// Returns the line and column represented by the given source location.
        ///
        /// If \p BufferID is provided, \p Loc must come from that source buffer.
        ///
        /// This respects #line directives.
        std::pair<unsigned, unsigned> getLineAndColumn(location loc, unsigned buffer_id = 0) const {
            assert(Loc.isValid());
            int line_offset = getLineOffset(loc);
            int l, c;
            std::tie(l, c) = m_source_manager.getLineAndColumn(loc.m_value, buffer_id);
            assert(line_offset + l > 0 && "bogus line offset");
            return { line_offset + l, c };
        }

        /// Returns the real line number for a source location.
        ///
        /// If \p BufferID is provided, \p Loc must come from that source buffer.
        ///
        /// This does not respect #line directives.
        unsigned getLineNumber(location loc, unsigned buffer_id = 0) const {
            assert(loc.isValid());
            return m_source_manager.FindLineNumber(loc.m_value, buffer_id);
        }

        StringRef extractText(CharSourceRange Range, Optional<unsigned> buffer_id = None) const;

        llvm::SMDiagnostic getMessage(location loc, llvm::SourceMgr::DiagKind kind, const Twine& msg, ArrayRef<llvm::SMRange> ranges, ArrayRef<llvm::SMFixIt> fix_its) const;

        /// Verifies that all buffers are still valid.
        void verifyAllBuffers() const;

        /// Translate line and column pair to the offset.
        llvm::Optional<unsigned> resolveFromLineCol(unsigned buffer_id, unsigned line, unsigned col) const;

        location getLocForLineCol(unsigned buffer_id, unsigned line, unsigned col) const {
            auto offset = resolveFromLineCol(buffer_id, line, col);
            return offset.hasValue() ? getLocForOffset(buffer_id, offset.getValue()) : location();
        }
    };

} // end of source namespace
} // end of hyperscale namespace
