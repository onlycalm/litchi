list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(PCRE2_PKT "${TLCHN_DIR}/pkt/linux/pcre2/pcre2-10.39.tar.gz")
set(PCRE2_INSTL_DIR "${TLCHN_DIR}/pcre2")

if(NOT EXISTS "${PCRE2_INSTL_DIR}")
    if(NOT EXISTS "${PCRE2_PKT}")
        PrintLog(FATAL_ERROR "pcre2 package not found at ${PCRE2_PKT}")
    endif()

    PrintLog(STATUS "Extracting pcre2...")

    file(MAKE_DIRECTORY ${PCRE2_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${PCRE2_PKT}"
        WORKING_DIRECTORY "${PCRE2_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${PCRE2_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract pcre2 package! Result: ${TAR_RESULT}")
    endif()
endif()
