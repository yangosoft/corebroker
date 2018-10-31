cmake_minimum_required(VERSION 3.4)

project(LibNodeMessage VERSION ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR} LANGUAGES CXX)

set( LibNodeMessageName "nodemessage" CACHE STRING "Lib nodemessagename")

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


set( nodemessage_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE STRING "Include directories for libfile" FORCE )
set( nodemessage_PUBLIC_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/api" CACHE STRING "Include directories for libfile public" FORCE )


message( "Library ${LibNodeMessageName} ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR}")
message("Files " ${SRC_FILES})
message("Headers " ${HEADER_FILES})
message("Public headers " ${PUBLIC_HEADER_FILES})




add_library( ${LibNodeMessageName} SHARED ${SRC_FILES}   )

target_include_directories(${LibNodeMessageName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../jsoncpp/")
target_include_directories(${LibNodeMessageName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_include_directories(${LibNodeMessageName} PRIVATE "${filelib_PUBLIC_INCLUDE_DIRS}")

target_include_directories(${LibNodeMessageName} PRIVATE src)
target_include_directories(${LibNodeMessageName} PRIVATE inc)
target_include_directories(${LibNodeMessageName} PRIVATE api)



set_target_properties( ${LibNodeMessageName}  PROPERTIES  VERSION ${CorebrokerCommonLibs_VERSION_MAJOR} SOVERSION ${CorebrokerCommonLibs_VERSION_MINOR} PUBLIC_HEADER "${PUBLIC_HEADER_FILES}" )

install(TARGETS ${LibNodeMessageName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/corebroker/message)

    
if( ENABLE_TEST )
    message("Unit test enabled ${CMAKE_CURRENT_SOURCE_DIR}/test/")
#    include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
endif()
