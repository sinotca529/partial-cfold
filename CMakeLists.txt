cmake_minimum_required(VERSION 3.13)

project(partial-cfold)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

find_package(LLVM
    REQUIRED
    CONFIG
    HINTS "$ENV{LLVM15}/lib/cmake"
    NO_DEFAULT_PATH
)

message(STATUS "Using LLVM ${LLVM_PACKAGE_VERSION}")

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIR})

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_llvm_pass_plugin(PartialCFold PartialCFold.cpp)

enable_testing()
add_test(
    NAME test1
    COMMAND "test1.sh"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test
)
