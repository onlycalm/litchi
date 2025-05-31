if(WIN32)
    execute_process(COMMAND powershell -Command "(Get-Date).Ticks / 10000"
        OUTPUT_VARIABLE start_time_ms
        OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    execute_process(COMMAND date +%s%3N
        OUTPUT_VARIABLE start_time_ms
        OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

cmake_minimum_required(VERSION 3.22.1)

project(litchi VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_GREATER 3.24)
    set(CMAKE_TIMESTAMP_FORMAT "[%Y-%m-%d %H:%M:%S] ")
else()
    set(CMAKE_C_COMPILER_LAUNCHER   "bash" "-c" "echo [$(date '+%Y-%m-%d %H:%M:%S')] && exec $0 $@")
    set(CMAKE_CXX_COMPILER_LAUNCHER "bash" "-c" "echo [$(date '+%Y-%m-%d %H:%M:%S')] && exec $0 $@")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/c++/11 -I/usr/include/x86_64-linux-gnu/c++/11")

set(BUILD_LOG "${CMAKE_BINARY_DIR}/build.log")
set(SCR_CHORE_PATH "${CMAKE_SOURCE_DIR}/scr/chore")
set(ANALYZE_BUILD_LOG_JSON "${CMAKE_BINARY_DIR}/AnlsBldLog.json")
set(COMPIL_TM_JSON "${CMAKE_BINARY_DIR}/CompilTm.json")
set(ANALYZE_BUILD_LOG_PY "${SCR_CHORE_PATH}/AnlsBldLog.py")
set(PRINT_BANNER_PY "${SCR_CHORE_PATH}/PrintBanner.py")
set(REC_COMPIL_TM_PY "${SCR_CHORE_PATH}/RecCompilTm.py")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -fdiagnostics-color=always
    )
endif()

include_directories(
    ./src
    ./src/com
    ./src/log
    ./src/drv/tcp
    ./src/win
)

aux_source_directory(./src SRC_SOURCES)
aux_source_directory(./src/com COM_SOURCES)
aux_source_directory(./src/log LOG_SOURCES)
aux_source_directory(./src/drv/tcp TCP_SOURCES)
aux_source_directory(./src/win WIN_SOURCES)

set(SRC_LIST
    ${SRC_SOURCES}
    ${COM_SOURCES}
    ${LOG_SOURCES}
    ${TCP_SOURCES}
    ${WIN_SOURCES}
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

qt_add_executable(litchi ${SRC_LIST})

target_link_libraries(litchi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

list(APPEND EXTRA_CLEAN_FILES
    "${BUILD_LOG}"
    "${ANALYZE_BUILD_LOG_JSON}"
)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY ADDITIONAL_CLEAN_FILES "${EXTRA_CLEAN_FILES}")

add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -E echo "=== Starting debug build ===" | tee -a ${BUILD_LOG}
    COMMAND ${Python3_EXECUTABLE} ${REC_COMPIL_TM_PY} "Begin" "${COMPIL_TM_JSON}" "${BUILD_LOG}"
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR} && ${CMAKE_COMMAND} --build . 2>&1 | tee -a "${BUILD_LOG}"
    COMMAND ${Python3_EXECUTABLE} ${ANALYZE_BUILD_LOG_PY} "Debug" "${BUILD_LOG}" "${ANALYZE_BUILD_LOG_JSON}"
    COMMAND ${Python3_EXECUTABLE} ${PRINT_BANNER_PY} "${ANALYZE_BUILD_LOG_JSON}" "${BUILD_LOG}"
    COMMAND ${Python3_EXECUTABLE} ${REC_COMPIL_TM_PY} "End" "${COMPIL_TM_JSON}" "${BUILD_LOG}"
    COMMAND bash -c "echo \"Compilation completion time: $(date '+%Y-%m-%d %H:%M:%S')\" | tee -a ${BUILD_LOG}"
    VERBATIM
)

add_custom_target(release
    COMMAND ${CMAKE_COMMAND} -E echo "=== Starting release build ===" | tee -a ${BUILD_LOG}
    COMMAND ${Python3_EXECUTABLE} ${REC_COMPIL_TM_PY} "Begin" "${COMPIL_TM_JSON}" "${BUILD_LOG}"
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR} && ${CMAKE_COMMAND} --build . 2>&1 | tee -a "${BUILD_LOG}"
    COMMAND ${Python3_EXECUTABLE} ${ANALYZE_BUILD_LOG_PY} "Release" "${BUILD_LOG}" "${ANALYZE_BUILD_LOG_JSON}"
    COMMAND ${Python3_EXECUTABLE} ${PRINT_BANNER_PY} "${ANALYZE_BUILD_LOG_JSON}" "${BUILD_LOG}"
    COMMAND ${Python3_EXECUTABLE} ${REC_COMPIL_TM_PY} "End" "${COMPIL_TM_JSON}" "${BUILD_LOG}"
    COMMAND bash -c "echo \"Compilation completion time: $(date '+%Y-%m-%d %H:%M:%S')\" | tee -a ${BUILD_LOG}"
    VERBATIM
)

if(WIN32)
    execute_process(COMMAND powershell -Command "(Get-Date).Ticks / 10000"
        OUTPUT_VARIABLE end_time_ms
        OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    execute_process(COMMAND date +%s%3N
        OUTPUT_VARIABLE end_time_ms
        OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

math(EXPR elapsed_ms "${end_time_ms} - ${start_time_ms}")

math(EXPR seconds "${elapsed_ms} / 1000")
math(EXPR milliseconds "${elapsed_ms} % 1000")

message(STATUS "Building cmake takes time: ${seconds}.${milliseconds}s")
