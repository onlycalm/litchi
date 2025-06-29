list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(LIBPNG_PKT "${TLCHN_DIR}/pkt/linux/libpng/libpng-1.6.37.tar.gz")
set(LIBPNG_INSTL_DIR "${TLCHN_DIR}/libpng")

if(NOT EXISTS "${LIBPNG_INSTL_DIR}")
    if(NOT EXISTS "${LIBPNG_PKT}")
        PrintLog(FATAL_ERROR "libpng package not found at ${LIBPNG_PKT}")
    endif()

    PrintLog(STATUS "Extracting libpng ...")

    file(MAKE_DIRECTORY ${LIBPNG_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${LIBPNG_PKT}"
        WORKING_DIRECTORY "${LIBPNG_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${LIBPNG_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract libpng package! Result: ${TAR_RESULT}")
    endif()
endif()
