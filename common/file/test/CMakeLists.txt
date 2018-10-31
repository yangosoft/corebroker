cmake_minimum_required(VERSION 3.11)

project(TestFile VERSION 1.0 LANGUAGES CXX)

set( TestFileName "testfile")

include (GNUInstallDirs)

set (SRC_FILES "")
set (HEADER_FILES "")

file(GLOB SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/test/src/*.cpp"
    )

file(GLOB HEADER_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/test/inc/*.h"
    )





include_directories(${COREBROKER_PRJ_PATH}/common/)
include_directories(${filelib_INCLUDE_DIRS})

message("${TestFileName} ${SRC_FILES}")

add_executable( ${TestFileName} ${SRC_FILES} )


target_link_libraries(${TestFileName} ${LibFileName})

message("LINK LIBS ${LibFile} ${LibFileName}")






