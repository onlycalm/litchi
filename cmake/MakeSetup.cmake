list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(MAKE_PKT "${TLCHN_DIR}/pkt/linux/make/make-4.3.tar.gz")
set(MAKE_INSTL_DIR "${TLCHN_DIR}/make")

if(NOT EXISTS "${MAKE_INSTL_DIR}")
    if(NOT EXISTS "${MAKE_PKT}")
        PrintLog(FATAL_ERROR "make package not found at ${MAKE_PKT}")
    endif()

    PrintLog(STATUS "Extracting make...")

    file(MAKE_DIRECTORY ${MAKE_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${MAKE_PKT}"
        WORKING_DIRECTORY "${MAKE_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${MAKE_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract make package! Result: ${TAR_RESULT}")
    endif()
endif()
