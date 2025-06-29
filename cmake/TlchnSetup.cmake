list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

set(TLCHN_URL "https://github.com/onlycalm/tlchn_litchi.git" CACHE STRING "Toolchain subrepo URL")
set(TLCHN_DIR "${CMAKE_SOURCE_DIR}/tlchn" CACHE PATH "Toolchain subrepo directory")
set(TLCHN_NM "tlchn")

if(NOT EXISTS "${TLCHN_DIR}/.git")
    PrintLog(STATUS "Cloning toolchain subrepo...")

    execute_process(
        COMMAND git submodule update --init --depth 1 ${TLCHN_NM}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_RESULT
        OUTPUT_QUIET ERROR_QUIET
    )

    if(NOT GIT_RESULT EQUAL 0)
        PrintLog(FATAL_ERROR "Failed to clone toolchain subrepo. Check URL or network.")
    endif()
endif()
