cmake_minimum_required(VERSION 3.20)

project(psuc_labs
    VERSION 1.0.0
    DESCRIPTION "Problem Solving Using C++ lab programs: 1-D arrays, 2-D arrays and strings"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(PSUC_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
option(PSUC_SANITIZE "Build with AddressSanitizer + UndefinedBehaviorSanitizer" OFF)
option(PSUC_BUILD_TESTS "Build the unit and end-to-end tests" ON)

# ---------------------------------------------------------------------------
# Common compiler settings
# ---------------------------------------------------------------------------
add_library(psuc_options INTERFACE)
if(MSVC)
    target_compile_options(psuc_options INTERFACE /W4 /permissive-)
    if(PSUC_WARNINGS_AS_ERRORS)
        target_compile_options(psuc_options INTERFACE /WX)
    endif()
else()
    target_compile_options(psuc_options INTERFACE
        -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion
        -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wnull-dereference)
    if(PSUC_WARNINGS_AS_ERRORS)
        target_compile_options(psuc_options INTERFACE -Werror)
    endif()
    if(PSUC_SANITIZE)
        target_compile_options(psuc_options INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(psuc_options INTERFACE -fsanitize=address,undefined)
    endif()
endif()

# ---------------------------------------------------------------------------
# Core library: the algorithms, independent of any I/O
# ---------------------------------------------------------------------------
add_library(psuc
    src/lib/arrays.cpp
    src/lib/matrix.cpp
    src/lib/strings.cpp
    src/lib/cli.cpp)
target_include_directories(psuc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(psuc PUBLIC psuc_options)

# ---------------------------------------------------------------------------
# One executable per lab exercise, all placed in <build>/bin
# ---------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB PSUC_PROGRAM_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/programs/*/*.cpp)

set(PSUC_PROGRAMS "")
foreach(source IN LISTS PSUC_PROGRAM_SOURCES)
    get_filename_component(program ${source} NAME_WE)
    add_executable(${program} ${source})
    target_link_libraries(${program} PRIVATE psuc)
    list(APPEND PSUC_PROGRAMS ${program})
endforeach()

install(TARGETS ${PSUC_PROGRAMS} RUNTIME DESTINATION bin)

# ---------------------------------------------------------------------------
# Developer tooling: `cmake --build build --target format|format-check|tidy`
# ---------------------------------------------------------------------------
file(GLOB_RECURSE PSUC_ALL_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.hpp)

find_program(CLANG_FORMAT_EXE NAMES clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${PSUC_ALL_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting sources with clang-format")
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror ${PSUC_ALL_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Checking formatting with clang-format")
endif()

find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    file(GLOB_RECURSE PSUC_TIDY_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} --quiet --warnings-as-errors=* ${PSUC_TIDY_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running clang-tidy")
endif()
