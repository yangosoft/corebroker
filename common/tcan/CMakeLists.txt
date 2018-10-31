cmake_minimum_required(VERSION 3.4)

project(LibCanSocket VERSION ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR} LANGUAGES CXX)

set( LibCanSocketName "libcansocket" CACHE STRING "Lib libcansocketname")

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


set( libcansocket_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE STRING "Include directories for libcansocket" FORCE )
set( libcansocket_PUBLIC_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/api" CACHE STRING "Include directories for libcansocket public" FORCE )


message(${CMAKE_CURRENT_SOURCE_DIR})
message( "Library  ${LibCanSocketName} ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR}")
message("Files " ${SRC_FILES})
message("Headers " ${HEADER_FILES})
message("Public headers " ${PUBLIC_HEADER_FILES})




add_library( ${LibCanSocketName} SHARED ${SRC_FILES}   )

target_link_libraries(${LibCanSocketName} pthread)

target_include_directories(${LibCanSocketName} PRIVATE ${libcansocket_INCLUDE_DIRS})
target_include_directories(${LibCanSocketName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../jsoncpp/")
target_include_directories(${LibCanSocketName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

target_include_directories(${LibCanSocketName} PRIVATE src)
target_include_directories(${LibCanSocketName} PRIVATE inc)
target_include_directories(${LibCanSocketName} PRIVATE api)



set_target_properties( ${LibCanSocketName}  PROPERTIES  VERSION ${CorebrokerCommonLibs_VERSION_MAJOR} SOVERSION ${CorebrokerCommonLibs_VERSION_MINOR} PUBLIC_HEADER "${PUBLIC_HEADER_FILES}" )

install(TARGETS ${LibCanSocketName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/corebroker/tcan)

    
if( ENABLE_TEST )
    message("Unit test enabled ${CMAKE_CURRENT_SOURCE_DIR}/test/")
#    include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
endif()
message("")
