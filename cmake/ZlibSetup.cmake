list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(ZLIB_PKT "${TLCHN_DIR}/pkt/linux/zlib/zlib-1.2.11.tar.gz")
set(ZLIB_INSTL_DIR "${TLCHN_DIR}/zlib")

if(NOT EXISTS "${ZLIB_INSTL_DIR}")
    if(NOT EXISTS "${ZLIB_PKT}")
        PrintLog(FATAL_ERROR "Python package not found at ${ZLIB_PKT}")
    endif()

    PrintLog(STATUS "Extracting zlib...")

    file(MAKE_DIRECTORY ${ZLIB_INSTL_DIR})

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf "${ZLIB_PKT}"
        WORKING_DIRECTORY "${ZLIB_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${ZLIB_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract zlib package! Result: ${TAR_RESULT}")
    endif()
endif()
