list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(MESA_PKT "${TLCHN_DIR}/pkt/linux/mesa/mesa-23.2.1.tar.gz")
set(MESA_INSTL_DIR "${TLCHN_DIR}/mesa")

if(NOT EXISTS "${MESA_INSTL_DIR}")
    if(NOT EXISTS "${MESA_PKT}")
        PrintLog(FATAL_ERROR "mesa package not found at ${MESA_PKT}")
    endif()

    PrintLog(STATUS "Extracting mesa...")

    file(MAKE_DIRECTORY ${MESA_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${MESA_PKT}"
        WORKING_DIRECTORY "${MESA_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${MESA_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract mesa package! Result: ${TAR_RESULT}")
    endif()
endif()
