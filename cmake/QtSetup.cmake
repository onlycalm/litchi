list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(QT_PKT "${TLCHN_DIR}/pkt/linux/qt/qt-6.9.0/qt-6.9.0.tar.xz")
set(QT_PKT_NM "qt-6.9.0.tar.xz")
set(QT_PKT_PART_NM "qt-6.9.0_part_*")
set(QT_PART_DIR "${TLCHN_DIR}/pkt/linux/qt/qt-6.9.0")
set(QT_INSTL_DIR "${TLCHN_DIR}/qt")

if(NOT EXISTS "${QT_INSTL_DIR}")
    if(NOT EXISTS "${QT_PART_DIR}")
        PrintLog(FATAL_ERROR "Qt package not found at ${QT_PART_DIR}")
    endif()

    PrintLog(STATUS "Extracting Qt...")

    file(MAKE_DIRECTORY ${QT_INSTL_DIR})

    execute_process(
        COMMAND bash -c "cd ${QT_PART_DIR} && cat ${QT_PKT_PART_NM} > ${QT_PKT_NM}"
        RESULT_VARIABLE result
        OUTPUT_QUIET
        ERROR_QUIET
    )

    execute_process(
        COMMAND bash -c "tar -xvjf ${QT_PKT} -C ${QT_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
        OUTPUT_QUIET
        ERROR_QUIET
    )

    file(REMOVE_RECURSE ${QT_PKT})

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${QT_INSTL_DIR})
        PrintLog(FATAL_ERROR "Failed to extract Qt package! Result: ${TAR_RESULT}")
    endif()

endif()
