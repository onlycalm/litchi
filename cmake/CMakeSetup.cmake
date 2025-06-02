list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Util)

if(NOT DEFINED TLCHN_DIR)
    PrintLog(FATAL_ERROR "Toolchain subrepo directory not found.")
endif()

set(CMAKE_PKT "${TLCHN_DIR}/pkt/linux/cmake/cmake-3.22.1.tar.gz")
set(CMAKE_INSTL_DIR "${TLCHN_DIR}/cmake")

if(NOT EXISTS "${CMAKE_INSTL_DIR}")
    if(NOT EXISTS "${CMAKE_PKT}")
        PrintLog(FATAL_ERROR "CMake package not found at ${CMAKE_PKT}")
    endif()

    PrintLog(STATUS "Extracting CMake...")

    file(MAKE_DIRECTORY ${CMAKE_INSTL_DIR})

    execute_process(
        COMMAND tar -xzf "${CMAKE_PKT}"
        WORKING_DIRECTORY "${CMAKE_INSTL_DIR}"
        RESULT_VARIABLE TAR_RESULT
    )

    if(NOT TAR_RESULT EQUAL 0)
        file(REMOVE_RECURSE ${CMAKE_INSTL_DIR})
        PrintLog(FATAL_ERROR "Failed to extract CMake package! Result: ${TAR_RESULT}")
    endif()
endif()
