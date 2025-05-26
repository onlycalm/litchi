cmake_minimum_required(VERSION 3.22.1)

project(litchi VERSION 0.1 LANGUAGES CXX C)

# Build compile_commands.json
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
set(ANALYZE_BUILD_LOG_SH "${CMAKE_SOURCE_DIR}/scr/chore/analyze_build_log.sh")
set(PRINT_BANNER_SH "${CMAKE_SOURCE_DIR}/scr/chore/print_banner.sh")
set(ANALYZE_BUILD_LOG_JSON "${CMAKE_BINARY_DIR}/analyze_build_log.json")

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

qt_add_executable(litchi ${SRC_LIST})

target_link_libraries(litchi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -E echo "=== Starting debug build ===" | tee -a ${BUILD_LOG}
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR} && ${CMAKE_COMMAND} --build . 2>&1 | tee -a "${BUILD_LOG}"
    COMMAND bash "${ANALYZE_BUILD_LOG_SH}" "${BUILD_LOG}" "Debug" "${ANALYZE_BUILD_LOG_JSON}"
    COMMAND bash "${PRINT_BANNER_SH}" "${ANALYZE_BUILD_LOG_JSON}" "${BUILD_LOG}"
    COMMAND date "+%Y-%m-%d %H:%M:%S"
    VERBATIM
)

add_custom_target(release
    COMMAND ${CMAKE_COMMAND} -E echo "=== Starting release build ===" | tee -a ${BUILD_LOG}
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR} && ${CMAKE_COMMAND} --build . 2>&1 | tee -a "${BUILD_LOG}"
    COMMAND bash "${ANALYZE_BUILD_LOG_SH}" "${BUILD_LOG}" "Release" "${ANALYZE_BUILD_LOG_JSON}"
    COMMAND bash "${PRINT_BANNER_SH}" "${ANALYZE_BUILD_LOG_JSON}" "${BUILD_LOG}"
    COMMAND date "+%Y-%m-%d %H:%M:%S"
    VERBATIM
)
