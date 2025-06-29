list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(GCC_PKT "${TLCHN_DIR}/pkt/linux/gcc/gcc-11.4.0/gcc-11.4.0.tar.xz")
set(GCC_PKT_NM "gcc-11.4.0.tar.xz")
set(GCC_PKT_PART_NM "gcc-11.4.0_part_*")
set(GCC_PART_DIR "${TLCHN_DIR}/pkt/linux/gcc/gcc-11.4.0")
set(GCC_INSTL_DIR "${TLCHN_DIR}/gcc")

if(NOT EXISTS "${GCC_INSTL_DIR}")
    if(NOT EXISTS "${GCC_PART_DIR}")
        PrintLog(FATAL_ERROR "gcc package not found at ${GCC_PART_DIR}")
    endif()

    PrintLog(STATUS "Extracting gcc...")

    file(MAKE_DIRECTORY ${GCC_INSTL_DIR})

    execute_process(
        COMMAND bash -c "cd ${GCC_PART_DIR} && cat ${GCC_PKT_PART_NM} > ${GCC_PKT_NM}"
        RESULT_VARIABLE result
        OUTPUT_QUIET
        ERROR_QUIET
    )

    execute_process(
        COMMAND bash -c "tar -xvJf ${GCC_PKT} -C ${GCC_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
        OUTPUT_QUIET
        ERROR_QUIET
    )

    file(REMOVE_RECURSE ${GCC_PKT})

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${GCC_INSTL_DIR})
        PrintLog(FATAL_ERROR "Failed to extract gcc package! Result: ${TAR_RESULT}")
    endif()

endif()
