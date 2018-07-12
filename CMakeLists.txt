cmake_minimum_required(VERSION 3.4)
set (CorebrokerCommonLibs_VERSION_MAJOR 1)
set (CorebrokerCommonLibs_VERSION_MINOR 0)
project(CorebrokerCommonLibs VERSION ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wsign-promo -Wnon-virtual-dtor -Wctor-dtor-privacy -Woverloaded-virtual -Wold-style-cast -Wpointer-arith -Wshadow -Wunused -Wuninitialized -Winit-self -Wdeprecated -Wfloat-equal")
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(COREBROKER_PRJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})


set(ENABLE_TEST OFF CACHE BOOL "Enable Google Test")

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug CACHE STRING  "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."       FORCE )
endif()

if( ENABLE_TEST )
    message("Unit test enabled")
    enable_testing()
endif()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/file)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/tmessage)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common/tnodenet)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/exampleclient)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simpleserver)
