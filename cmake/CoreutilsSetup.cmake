list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(COREUTILS_PKT "${TLCHN_DIR}/pkt/linux/coreutils/coreutils-8.32.tar.gz")
set(COREUTILS_INSTL_DIR "${TLCHN_DIR}/coreutils")

if(NOT EXISTS "${COREUTILS_INSTL_DIR}")
    if(NOT EXISTS "${COREUTILS_PKT}")
        PrintLog(FATAL_ERROR "coreutils package not found at ${COREUTILS_PKT}")
    endif()

    PrintLog(STATUS "Extracting coreutils...")

    file(MAKE_DIRECTORY ${COREUTILS_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${COREUTILS_PKT}"
        WORKING_DIRECTORY "${COREUTILS_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${COREUTILS_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract coreutils package! Result: ${TAR_RESULT}")
    endif()
endif()
