cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
    "ShellProject"
    VERSION 1.0.0
    DESCRIPTION "A nice shell project!"
    LANGUAGES C
)

enable_testing()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "-g3 -Wall -pedantic -Werror -Wextra -Wconversion")

include_directories(include)

if(EXISTS "${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake")
    include(${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake)
else()
    message(FATAL_ERROR "Conan toolchain file not found at ${CMAKE_BINARY_DIR}/Release/generators/conan_toolchain.cmake")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("-- Building Release mode with code coverage enabled")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
endif()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/generators")

find_package(cJSON REQUIRED)
find_package(unity REQUIRED)

add_subdirectory(submodule)

add_executable(${PROJECT_NAME}
    src/main.c
    src/commands.c
    src/execution.c
    src/jobs.c
    src/global.c
    src/utils.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE cjson::cjson unity::unity)

add_subdirectory(tests)
