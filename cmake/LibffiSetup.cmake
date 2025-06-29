list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(LIBFFI_PKT "${TLCHN_DIR}/pkt/linux/libffi/libffi-3.4.2.tar.gz")
set(LIBFFI_INSTL_DIR "${TLCHN_DIR}/libffi")

if(NOT EXISTS "${LIBFFI_INSTL_DIR}")
    if(NOT EXISTS "${LIBFFI_PKT}")
        PrintLog(FATAL_ERROR "libffi package not found at ${LIBFFI_PKT}")
    endif()

    PrintLog(STATUS "Extracting libffi...")

    file(MAKE_DIRECTORY ${LIBFFI_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${LIBFFI_PKT}"
        WORKING_DIRECTORY "${LIBFFI_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${LIBFFI_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract libffi package! Result: ${TAR_RESULT}")
    endif()
endif()
