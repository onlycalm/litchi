if(WIN32)
    execute_process(COMMAND powershell -Command "(Get-Date).Ticks / 10000"
        OUTPUT_VARIABLE start_time_ms
        OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    execute_process(COMMAND date +%s%3N
        OUTPUT_VARIABLE start_time_ms
        OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

if(CMAKE_VERSION VERSION_GREATER 3.24)
    set(CMAKE_TIMESTAMP_FORMAT "[%Y-%m-%d %H:%M:%S] ")
else()
    set(CMAKE_C_COMPILER_LAUNCHER   "bash" "-c" "echo [$(date '+%Y-%m-%d %H:%M:%S')] && exec $0 $@")
    set(CMAKE_CXX_COMPILER_LAUNCHER "bash" "-c" "echo [$(date '+%Y-%m-%d %H:%M:%S')] && exec $0 $@")
endif()

set(TLCHN_DIR "${CMAKE_SOURCE_DIR}/tlchn")
set(BUILD_LOG "${CMAKE_BINARY_DIR}/build.log")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CORE_UTILS_ROOT "${TLCHN_DIR}/coreutils/coreutils-8.32")
set(MESA_ROOT "${TLCHN_DIR}/mesa/mesa-23.2.1")
set(MAKE_ROOT "${TLCHN_DIR}/make/make-4.3")
set(GCC_ROOT "${TLCHN_DIR}/gcc/gcc-11.4.0")
set(CMAKE_FIND_ROOT_PATH "${TLCHN_DIR}")
set(CMAKE_DIR "${TLCHN_DIR}/cmake/cmake-3.22.1")

set(OpenGL_DIR "${MESA_ROOT}/lib/cmake/OpenGL")
set(OpenGL_GL_PREFIX "${MESA_ROOT}")
set(OpenGL_INCLUDE_DIR "${OpenGL_GL_PREFIX}/include")
set(OpenGL_gl_LIBRARY "${OpenGL_GL_PREFIX}/lib/libGL.so")
set(OpenGL_glx_LIBRARY "${OpenGL_GL_PREFIX}/lib/libGLX.so")
set(EGL_INCLUDE_DIR "${MESA_ROOT}/include")
set(EGL_LIBRARY "${MESA_ROOT}/lib/libEGL.so")
set(CMAKE_UNAME "${CORE_UTILS_ROOT}/bin/uname")
set(MAKE_DIR "${MAKE_ROOT}/bin/make")
set(TOOLCHAIN_DIR "${GCC_ROOT}")
set(LD_LIBRARY_PATH "${GCC_ROOT}/lib64")
set(CMAKE_AR "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-ar")
set(CMAKE_C_COMPILER_AR "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-ar")
set(CMAKE_CXX_COMPILER_AR "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-ar")
set(CMAKE_RANLIB "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-ranlib")
set(CMAKE_C_COMPILER_RANLIB "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-ranlib")
set(CMAKE_CXX_COMPILER_RANLIB "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-ranlib")
set(CMAKE_NM "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-nm")
set(CMAKE_STRIP "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-strip")
set(CMAKE_LINKER "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-ld")
set(CMAKE_OBJCOPY "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-objcopy")
set(CMAKE_OBJDUMP "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-objdump")
set(CMAKE_READELF "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-readelf")
set(CMAKE_ADDR2LINE "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-addr2line")
set(CMAKE_DLLTOOL "${TOOLCHAIN_DIR}/bin/x86_64-linux-gnu-dlltool")
set(CMAKE_ROOT "${CMAKE_DIR}/share/cmake-3.22")
set(CMAKE_C_COMPILER "${GCC_ROOT}/bin/x86_64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "${GCC_ROOT}/bin/x86_64-linux-gnu-g++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_ROOT}/Modules")
list(APPEND CMAKE_FIND_ROOT_PATH "${GCC_ROOT}")

set(CMAKE_GENERATOR "Unix Makefiles")
set(CMAKE_MAKE_PROGRAM "${MAKE_DIR}" CACHE INTERNAL "" FORCE)
set(__FIND_MAKE_PROGRAM_PATHS "")

set(CMAKE_DISABLE_FIND_PACKAGE_Ninja TRUE)
set(CMAKE_DISABLE_FIND_PACKAGE_Makefile TRUE)

include(Util)
include(TlchnSetup)
include(ZlibSetup)
include(IcuSetup)
include(Pcre2Setup)
include(LibffiSetup)
include(PkgCfgSetup)
include(GlibSetup)
include(LibpngSetup)
include(LibbacktraceSetup)
include(MesaSetup)
include(CoreutilsSetup)
include(MakeSetup)
include(GccSetup)
include(CMakeSetup)
include(QtSetup)
include(PythonSetup)

cmake_minimum_required(VERSION 3.22.1)

project(litchi VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH OFF)
set(CMAKE_FIND_USE_CMAKE_SYSTEM_PATH OFF)
set(QT_NO_GLOBAL_CMAKE ON)
set(QT_NO_DEFAULT_CMAKE ON)
set(CMAKE_DISABLE_FIND_PACKAGE_OpenGL TRUE)
set(OpenGL_GL_PREFERENCE LEGACY)
set(Python3_FIND_REGISTRY NEVER)
set(Python3_FIND_STRATEGY LOCATION)
set(CMAKE_FIND_DEBUG_MODE ON)
set(QT_DEBUG_FIND_PACKAGE ON)

set(ZLIB_ROOT "${TLCHN_DIR}/zlib/zlib-1.2.11")
set(ICU_ROOT "${TLCHN_DIR}/icu/icu-70.1")
set(PCRE2_ROOT "${TLCHN_DIR}/pcre2/pcre2-10.39")
set(LIBFFI_ROOT "${TLCHN_DIR}/libffi/libffi-3.4.2")
set(PKG_CONFIG_ROOT "${TLCHN_DIR}/pkg-config/pkg-config-0.29.2")
set(GLIB2_ROOT "${TLCHN_DIR}/glib/glib-2.72.4")
set(PNG_ROOT "${TLCHN_DIR}/libpng/libpng-1.6.37")
set(BACKTRACE_ROOT "${TLCHN_DIR}/libbacktrace/libbacktrace-1.0")
set(QT6_ROOT "${TLCHN_DIR}/qt/qt-6.9.0")
set(PYTHON_ROOT "${TLCHN_DIR}/python/python-3.10.12")

set(PKG_CONFIG_EXECUTABLE "${PKG_CONFIG_ROOT}/bin/pkg-config")
set(GLIB2_INCLUDE_DIRS "${GLIB2_ROOT}/include")
set(GLIB2_LIBRARIES "${GLIB2_ROOT}/lib")
set(PNG_LIBRARY "${PNG_ROOT}/lib/libpng.so")
set(PNG_INCLUDE_DIR "${PNG_ROOT}/include")
set(Backtrace_INCLUDE_DIR "${BACKTRACE_ROOT}/include/")
set(Backtrace_LIBRARY "${BACKTRACE_ROOT}/lib/libbacktrace.a")
set(CMAKE_COMMAND "${CMAKE_DIR}/bin/cmake")
set(Qt6_DIR "${QT6_ROOT}/lib/cmake/Qt6")
set(Qt6Core_DIR "${QT6_ROOT}/lib/cmake/Qt6Core")
set(Qt6Widgets_DIR "${QT6_ROOT}/lib/cmake/Qt6Widgets")
set(Qt6Gui_DIR "${QT6_ROOT}/lib/cmake/Qt6Gui")
set(QT_QMAKE_EXECUTABLE "${QT6_ROOT}/bin/qmake")
set(QT_MOC_EXECUTABLE "${QT6_ROOT}/libexec/moc")
set(Python3_EXECUTABLE "${PYTHON_ROOT}/bin/python3.10")
set(PYTHON_INCLUDE_DIR "${PYTHON_ROOT}/include/python3.10")
set(PYTHON_LIBRARY "${PYTHON_ROOT}/lib/libpython3.10.so")

set(SCR_CHORE_PATH "${CMAKE_SOURCE_DIR}/scr/chore")
set(ANALYZE_BUILD_LOG_JSON "${CMAKE_BINARY_DIR}/AnlsBldLog.json")
set(COMPIL_TM_JSON "${CMAKE_BINARY_DIR}/CompilTm.json")
set(ANALYZE_BUILD_LOG_PY "${SCR_CHORE_PATH}/AnlsBldLog.py")
set(PRINT_BANNER_PY "${SCR_CHORE_PATH}/PrintBanner.py")
set(REC_COMPIL_TM_PY "${SCR_CHORE_PATH}/RecCompilTm.py")

set(ENV{PKG_CONFIG_PATH} "${LIBFFI_ROOT}/lib/pkgconfig:${PKG_CONFIG_ROOT}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
set(ENV{PKG_CONFIG_PATH} "${TLCHN_DIR}/mesa/mesa-23.2.1/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")

list(APPEND CMAKE_INSTALL_PREFIX "${TLCHN_DIR}")
set(CMAKE_PREFIX_PATH "${QT6_ROOT}")
list(APPEND CMAKE_PREFIX_PATH "${CORE_UTILS_ROOT}/bin")
list(PREPEND CMAKE_PREFIX_PATH "${TLCHN_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${PCRE2_ROOT}")
list(APPEND CMAKE_PREFIX_PATH "${MESA_ROOT}")
set(CMAKE_LIBRARY_PATH "${MESA_ROOT}/lib")
set(CMAKE_INCLUDE_PATH "${MESA_ROOT}/include")
list(APPEND CMAKE_MODULE_PATH "${QT6_ROOT}/lib/cmake/Qt6Core")
list(APPEND CMAKE_MODULE_PATH "${QT6_ROOT}/lib/cmake/Qt6Gui")
list(APPEND CMAKE_MODULE_PATH "${Qt6_DIR}")
set(CMAKE_PROGRAM_PATH "${CORE_UTILS_ROOT}/bin")
list(APPEND CMAKE_MODULE_PATH "${QT6_ROOT}/lib/cmake/Qt6Core")
list(APPEND CMAKE_MODULE_PATH "${QT6_ROOT}/lib/cmake/Qt6Gui")
list(APPEND CMAKE_MODULE_PATH "${QT6_ROOT}/lib/cmake/Qt6Widgets")
list(APPEND CMAKE_MODULE_PATH "${Qt6_DIR}")

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

include_directories(${PYTHON_INCLUDE_DIR})

include_directories(
    ./src
    ./src/com
    ./src/log
    ./src/drv/tcp
    ./src/win
    ./build/autogen/include
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

find_program(PROJECT_CMAKE cmake
  PATHS "${CMAKE_DIR}/bin"
  NO_DEFAULT_PATH
  NO_CMAKE_FIND_ROOT_PATH
)

add_library(OpenGL::GL UNKNOWN IMPORTED)
set_target_properties(OpenGL::GL PROPERTIES
  IMPORTED_LOCATION "${OpenGL_gl_LIBRARY}"
  INTERFACE_INCLUDE_DIRECTORIES "${OpenGL_INCLUDE_DIR}"
)

add_library(OpenGL::GLX UNKNOWN IMPORTED)
set_target_properties(OpenGL::GLX PROPERTIES
  IMPORTED_LOCATION "${OpenGL_glx_LIBRARY}"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenGL REQUIRED_VARS
  OpenGL_gl_LIBRARY OpenGL_INCLUDE_DIR
)

set(WrapOpenGL_FOUND TRUE)
set(OpenGL_FOUND TRUE)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Python3 COMPONENTS Interpreter REQUIRED)

pkg_check_modules(LIBFFI REQUIRED libffi)

qt_add_executable(litchi ${SRC_LIST})

target_link_libraries(litchi PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

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

file(READ "${ZLIB_ROOT}/include/zlib.h" ZLIB_H_CONTENTS)
string(REGEX MATCH "define ZLIB_VERSION \"[^\"]*\"" ZLIB_VERSION_DEFINE ${ZLIB_H_CONTENTS})
string(REGEX REPLACE "define ZLIB_VERSION \"([^\"]*)\"" "\\1" ZLIB_VERSION ${ZLIB_VERSION_DEFINE})

file(STRINGS "${PCRE2_ROOT}/include/pcre2.h" PCRE2_MAJOR_LINE REGEX "^#define PCRE2_MAJOR +[0-9]+")
string(REGEX REPLACE "^#define PCRE2_MAJOR +([0-9]+).*$" "\\1" PCRE2_MAJOR "${PCRE2_MAJOR_LINE}")
file(STRINGS "${PCRE2_ROOT}/include/pcre2.h" PCRE2_MINOR_LINE REGEX "^#define PCRE2_MINOR +[0-9]+")
string(REGEX REPLACE "^#define PCRE2_MINOR +([0-9]+).*$" "\\1" PCRE2_MINOR "${PCRE2_MINOR_LINE}")
set(PCRE2_VERSION "${PCRE2_MAJOR}.${PCRE2_MINOR}")

execute_process(
  COMMAND ${PKG_CONFIG_EXECUTABLE} --version
  OUTPUT_VARIABLE PKGCONFIG_REAL_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

find_file(PNG_HEADER png.h PATHS ${PNG_INCLUDE_DIR})
file(STRINGS ${PNG_HEADER} PNG_VERSION_MAJOR REGEX "^#define PNG_LIBPNG_VER_MAJOR +[0-9]+")
file(STRINGS ${PNG_HEADER} PNG_VERSION_MINOR REGEX "^#define PNG_LIBPNG_VER_MINOR +[0-9]+")
file(STRINGS ${PNG_HEADER} PNG_VERSION_RELEASE REGEX "^#define PNG_LIBPNG_VER_RELEASE +[0-9]+")
string(REGEX REPLACE "^#define PNG_LIBPNG_VER_MAJOR +([0-9]+)" "\\1" PNG_MAJOR ${PNG_VERSION_MAJOR})
string(REGEX REPLACE "^#define PNG_LIBPNG_VER_MINOR +([0-9]+)" "\\1" PNG_MINOR ${PNG_VERSION_MINOR})
string(REGEX REPLACE "^#define PNG_LIBPNG_VER_RELEASE +([0-9]+)" "\\1" PNG_RELEASE ${PNG_VERSION_RELEASE})
set(PNG_VERSION "${PNG_MAJOR}.${PNG_MINOR}.${PNG_RELEASE}")

execute_process(
    COMMAND bash -c "gcc --version | awk 'NR==1 {print $NF}'"
    OUTPUT_VARIABLE GCC_VERSION_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND bash -c "gcc --version | awk 'NR==1 {print $NF}'"
    OUTPUT_VARIABLE GXX_VERSION_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

find_program(CMAKE_UNAME uname)
if(CMAKE_UNAME)
    execute_process(
        COMMAND ${CMAKE_UNAME} --version
        OUTPUT_VARIABLE UNAME_VERSION_FULL
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    string(REGEX MATCH "uname \\(GNU coreutils\\) ([0-9]+\\.[0-9]+)"
           VERSION_MATCH "${UNAME_VERSION_FULL}")
    set(UNAME_VERSION ${CMAKE_MATCH_1})
else()
    message(WARNING "uname not found")
endif()

execute_process(
    COMMAND ${MAKE_DIR} --version
    COMMAND head -n 1
    OUTPUT_VARIABLE MAKE_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
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

PrintLog(STATUS "PATH: ${PATH}")
PrintLog(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
PrintLog(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
PrintLog(STATUS "LD_LIBRARY_PATH: ${LD_LIBRARY_PATH}")

PrintLog(STATUS "zlib path: ${ZLIB_ROOT}")
PrintLog(STATUS "zlib version: ${ZLIB_VERSION}")
PrintLog(STATUS "icu path: ${ICU_ROOT}")
PrintLog(STATUS "icu version: ${ICU_VERSION}")
PrintLog(STATUS "pcre2 path ${PCRE2_ROOT}")
PrintLog(STATUS "pcre2 library: ${PCRE2_LIBRARY}")
PrintLog(STATUS "pcre2 version: ${PCRE2_VERSION}")
PrintLog(STATUS "libffi path: ${LIBFFI_ROOT}")
PrintLog(STATUS "libffi version: ${LIBFFI_VERSION}")
PrintLog(STATUS "pkg-config path: ${PKG_CONFIG_ROOT}")
PrintLog(STATUS "pkg-config executable path: ${PKG_CONFIG_EXECUTABLE}")
PrintLog(STATUS "pkg-config version: ${PKGCONFIG_REAL_VERSION}")
PrintLog(STATUS "GLIB2 path: ${GLIB2_ROOT}")
PrintLog(STATUS "GLIB2_INCLUDE_DIRS: ${GLIB2_INCLUDE_DIRS}")
PrintLog(STATUS "GLIB2_LIBRARIES: ${GLIB2_LIBRARIES}")
PrintLog(STATUS "GLIB2 version: ${GLIB2_VERSION}")
PrintLog(STATUS "libpng path: ${PNG_ROOT}")
PrintLog(STATUS "libpng lib: ${PNG_LIBRARY}")
PrintLog(STATUS "libpng include: ${PNG_INCLUDE_DIR}")
PrintLog(STATUS "libpng version: ${PNG_VERSION}")
PrintLog(STATUS "libbacktrace path: ${BACKTRACE_ROOT}")
PrintLog(STATUS "libbacktrace include path: ${Backtrace_INCLUDE_DIR}")
PrintLog(STATUS "libbacktrace lib path: ${Backtrace_LIBRARY}")
PrintLog(STATUS "libbacktrace version: ${Backtrace_VERSION}")
PrintLog(STATUS "OpenGL include dir: ${OPENGL_INCLUDE_DIR}")
PrintLog(STATUS "OpenGL libraries: ${OpenGL_gl_LIBRARY}")
PrintLog(STATUS "OpenGL version: ${OPENGL_VERSION}")
PrintLog(STATUS "EGL include path: ${EGL_INCLUDE_DIR}")
PrintLog(STATUS "EGL lib path: ${EGL_LIBRARY}")
PrintLog(STATUS "EGL version: ${EGL_VERSION}")
PrintLog(STATUS "HarfBuzz include path: ${HarfBuzz_INCLUDE_DIR}")
PrintLog(STATUS "HarfBuzz lib path: ${HarfBuzz_LIBRARY}")
PrintLog(STATUS "HarfBuzz version: ${HarfBuzz_VERSION}")
PrintLog(STATUS "uname path: ${CMAKE_UNAME}")
PrintLog(STATUS "uname version: ${UNAME_VERSION}")
PrintLog(STATUS "make path: ${MAKE_DIR}")
PrintLog(STATUS "make version: ${MAKE_VERSION}")
PrintLog(STATUS "gcc path: ${CMAKE_C_COMPILER}")
PrintLog(STATUS "gcc version: ${GCC_VERSION_OUTPUT}")
PrintLog(STATUS "g++ path: ${CMAKE_CXX_COMPILER}")
PrintLog(STATUS "g++ version: ${GXX_VERSION_OUTPUT}")
PrintLog(STATUS "CMake path: ${CMAKE_COMMAND}")
PrintLog(STATUS "CMake version: ${CMAKE_VERSION}")
PrintLog(STATUS "Qt6 path: ${Qt6_DIR}")
PrintLog(STATUS "Qt6 core path: ${Qt6Core_DIR}")
PrintLog(STATUS "Qt6 version: ${Qt6_VERSION}")
PrintLog(STATUS "Python3 path: ${Python3_EXECUTABLE}")
PrintLog(STATUS "Python3 version: ${Python3_VERSION}")
PrintLog(STATUS "Building cmake takes time: ${seconds}.${milliseconds}s")
