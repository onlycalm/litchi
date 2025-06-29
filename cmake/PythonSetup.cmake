list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(PYTHON_PKT "${TLCHN_DIR}/pkt/linux/python/python-3.10.12.tar.gz")
set(PYTHON_INSTL_DIR "${TLCHN_DIR}/python")

if(NOT EXISTS "${PYTHON_INSTL_DIR}")
    if(NOT EXISTS "${PYTHON_PKT}")
        PrintLog(FATAL_ERROR "Python package not found at ${PYTHON_PKT}")
    endif()

    PrintLog(STATUS "Extracting Python...")

    file(MAKE_DIRECTORY ${PYTHON_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${PYTHON_PKT}"
        WORKING_DIRECTORY "${PYTHON_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${PYTHON_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract Python package! Result: ${TAR_RESULT}")
    endif()
endif()
