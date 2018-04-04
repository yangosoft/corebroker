cmake_minimum_required(VERSION 3.4)

project(LibNodeSocket VERSION ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR} LANGUAGES CXX)

set( LibNodeSocketName "nodesocket")

include (GNUInstallDirs)

file(GLOB SRC_FILES
    "src/*.cpp"
    )

file(GLOB HEADER_FILES
    "inc/*.h"
    )

file(GLOB PUBLIC_HEADER_FILES
    "api/*.h"
    )


set( filenodesocket_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE STRING "Include directories for libnodesocket" FORCE )
set( filenodesocket_PUBLIC_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/api" CACHE STRING "Include directories for libnodesocket public" FORCE )


message(${CMAKE_CURRENT_SOURCE_DIR})
message( "Library  ${LibNodeSocketName} ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR}")
message("Files " ${SRC_FILES})
message("Headers " ${HEADER_FILES})
message("Public headers " ${PUBLIC_HEADER_FILES})




add_library( ${LibNodeSocketName} SHARED ${SRC_FILES}   )



target_include_directories(${LibNodeSocketName} PRIVATE ${nodemessage_INCLUDE_DIRS})
target_include_directories(${LibNodeSocketName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../jsoncpp/")
target_include_directories(${LibNodeSocketName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

target_include_directories(${LibNodeSocketName} PRIVATE src)
target_include_directories(${LibNodeSocketName} PRIVATE inc)
target_include_directories(${LibNodeSocketName} PRIVATE api)



set_target_properties( ${LibNodeSocketName}  PROPERTIES  VERSION ${CorebrokerCommonLibs_VERSION_MAJOR} SOVERSION ${CorebrokerCommonLibs_VERSION_MINOR} PUBLIC_HEADER "${PUBLIC_HEADER_FILES}" )

install(TARGETS ${LibNodeSocketName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/corebroker/nodenet)

    
if( ENABLE_TEST )
    message("Unit test enabled ${CMAKE_CURRENT_SOURCE_DIR}/test/")
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
endif()
message("")
