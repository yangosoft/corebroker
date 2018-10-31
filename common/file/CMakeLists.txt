cmake_minimum_required(VERSION 3.4)

project(LibFile VERSION ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR} LANGUAGES CXX)

set( LibFileName "file" CACHE STRING "Lib filename")



include (GNUInstallDirs)

file(GLOB SRC_FILES
    "src/*.cpp"
    )

file(GLOB HEADER_FILES
    "inc/*.h"
    )

file(GLOB PUBLIC_HEADER_FILES
    "inc/*.h"
    )


set( filelib_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE STRING "Include directories for libfile" FORCE )
set( filelib_PUBLIC_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/api" CACHE STRING "Include directories for libfile public" FORCE )


message( "Library " ${LibFile} ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR})
message("Files " ${SRC_FILES})
message("Headers " ${HEADER_FILES})
message("Public headers " ${PUBLIC_HEADER_FILES})




add_library( ${LibFileName} SHARED ${SRC_FILES}   )

target_include_directories(${LibFileName} PRIVATE src)
target_include_directories(${LibFileName} PRIVATE inc)
target_include_directories(${LibFileName} PRIVATE api)



set_target_properties( ${LibFileName}  PROPERTIES  VERSION ${CorebrokerCommonLibs_VERSION_MAJOR} SOVERSION ${CorebrokerCommonLibs_VERSION_MINOR} PUBLIC_HEADER "${PUBLIC_HEADER_FILES}" )

install(TARGETS ${LibFileName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/corebroker/file)

    
if( ENABLE_TEST )
    message("Unit test enabled ${CMAKE_CURRENT_SOURCE_DIR}/test/")
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
endif()
