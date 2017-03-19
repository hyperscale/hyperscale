/**
 * Hyperscale
 *
 * (c) 2015-2016 Axel Etcheverry
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#pragma once

#include <hyperscale/config.hpp>
#include <einfo.h>
#include <llvm/Config/llvm-config.h>

namespace hyperscale {

    /**
     * Print version
     */
    inline void show_version() {
        einfo(
            "%s v%d.%d.%d (%s/%s)",
            HYPERSCALE_NAME,
            HYPERSCALE_VERSION_MAJOR,
            HYPERSCALE_VERSION_MINOR,
            HYPERSCALE_VERSION_PATCH,
            SYSTEM_NAME,
            SYSTEM_PROCESSOR
        );
    }

    /**
     * Print copyright
     */
    inline void show_copyright() {
        einfo(
            "Copyright (c) %d-%d %s",
            HYPERSCALE_COPYRIGHT_START_YEAR,
            HYPERSCALE_COPYRIGHT_END_YEAR,
            HYPERSCALE_AUTHOR
        );
    }

    /**
     * Print LLVM version
     */
    inline void show_llvm_version() {
        einfo(
            "LLVM v%d.%d",
            LLVM_VERSION_MAJOR,
            LLVM_VERSION_MINOR
        );
    }
} // end of hyperscale namespace

