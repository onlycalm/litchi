list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(LIBBACKTRACE_PKT "${TLCHN_DIR}/pkt/linux/libbacktrace/libbacktrace-1.0.tar.gz")
set(LIBBACKTRACE_INSTL_DIR "${TLCHN_DIR}/libbacktrace")

if(NOT EXISTS "${LIBBACKTRACE_INSTL_DIR}")
    if(NOT EXISTS "${LIBBACKTRACE_PKT}")
        PrintLog(FATAL_ERROR "libbacktrace package not found at ${LIBBACKTRACE_PKT}")
    endif()

    PrintLog(STATUS "Extracting libbacktrace...")

    file(MAKE_DIRECTORY ${LIBBACKTRACE_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${LIBBACKTRACE_PKT}"
        WORKING_DIRECTORY "${LIBBACKTRACE_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${LIBBACKTRACE_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract libbacktrace package! Result: ${TAR_RESULT}")
    endif()
endif()
