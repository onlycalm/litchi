list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(ICU_PKT "${TLCHN_DIR}/pkt/linux/icu/icu-70.1.tar.gz")
set(ICU_INSTL_DIR "${TLCHN_DIR}/icu")

if(NOT EXISTS "${ICU_INSTL_DIR}")
    if(NOT EXISTS "${ICU_PKT}")
        PrintLog(FATAL_ERROR "Icu package not found at ${ICU_PKT}")
    endif()

    PrintLog(STATUS "Extracting icu...")

    file(MAKE_DIRECTORY ${ICU_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${ICU_PKT}"
        WORKING_DIRECTORY "${ICU_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${ICU_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract icu package! Result: ${TAR_RESULT}")
    endif()
endif()
