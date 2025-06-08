list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(GLIB_PKT "${TLCHN_DIR}/pkt/linux/glib/glib-2.72.4.tar.gz")
set(GLIB_INSTL_DIR "${TLCHN_DIR}/glib")

if(NOT EXISTS "${GLIB_INSTL_DIR}")
    if(NOT EXISTS "${GLIB_PKT}")
        PrintLog(FATAL_ERROR "glib package not found at ${GLIB_PKT}")
    endif()

    PrintLog(STATUS "Extracting glib ...")

    file(MAKE_DIRECTORY ${GLIB_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${GLIB_PKT}"
        WORKING_DIRECTORY "${GLIB_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${GLIB_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract glib package! Result: ${TAR_RESULT}")
    endif()
endif()
