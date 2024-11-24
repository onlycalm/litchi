cmake_minimum_required(VERSION 3.6)

project(litchi VERSION 0.1 LANGUAGES CXX C)

# Build compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ./src
    ./src/com
    ./src/log
    ./src/drv/tcp
    ./src/drv/doip
    ./src/drv/uds
    ./src/win
)

aux_source_directory(./src SRC_SOURCES)
aux_source_directory(./src/com COM_SOURCES)
aux_source_directory(./src/log LOG_SOURCES)
aux_source_directory(./src/drv/tcp TCP_SOURCES)
aux_source_directory(./src/drv/doip DOIP_SOURCES)
aux_source_directory(./src/drv/uds UDS_SOURCES)
aux_source_directory(./src/win WIN_SOURCES)

set(SRC_LIST
    ${SRC_SOURCES}
    ${COM_SOURCES}
    ${LOG_SOURCES}
    ${TCP_SOURCES}
    ${DOIP_SOURCES}
    ${UDS_SOURCES}
    ${WIN_SOURCES}
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(litchi ${SRC_LIST})

target_link_libraries(litchi PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
