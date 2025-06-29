list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(PKG_CFG_PKT "${TLCHN_DIR}/pkt/linux/pkg-config/pkg-config-0.29.2.tar.gz")
set(PKG_CFG_INSTL_DIR "${TLCHN_DIR}/pkg-config")

if(NOT EXISTS "${PKG_CFG_INSTL_DIR}")
    if(NOT EXISTS "${PKG_CFG_PKT}")
        PrintLog(FATAL_ERROR "pkg-config package not found at ${PKG_CFG_PKT}")
    endif()

    PrintLog(STATUS "Extracting pkg-config...")

    file(MAKE_DIRECTORY ${PKG_CFG_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${PKG_CFG_PKT}"
        WORKING_DIRECTORY "${PKG_CFG_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${PKG_CFG_INSTL_DIR}) # remove install directory.
        PrintLog(FATAL_ERROR "Failed to extract pkg-config package! Result: ${TAR_RESULT}")
    endif()
endif()
