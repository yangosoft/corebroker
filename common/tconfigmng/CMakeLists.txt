cmake_minimum_required(VERSION 3.4)

project(LibConfigMng VERSION ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR} LANGUAGES CXX)

set( LibConfigMngName "libconfigmng" CACHE STRING "Lib libconfigmngname")

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


set( libconfigmng_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/inc" CACHE STRING "Include directories for libconfigmng" FORCE )
set( libconfigmng_PUBLIC_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/api" CACHE STRING "Include directories for libconfigmng public" FORCE )


message(${CMAKE_CURRENT_SOURCE_DIR})
message( "Library  ${LibConfigMngName} ${CorebrokerCommonLibs_VERSION_MAJOR}.${CorebrokerCommonLibs_VERSION_MINOR}")
message("Files " ${SRC_FILES})
message("Headers " ${HEADER_FILES})
message("Public headers " ${PUBLIC_HEADER_FILES})




add_library( ${LibConfigMngName} SHARED ${SRC_FILES}   )

target_link_libraries(${LibConfigMngName} pthread)

target_include_directories(${LibConfigMngName} PRIVATE ${libconfigmng_INCLUDE_DIRS})
target_include_directories(${LibConfigMngName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../jsoncpp/")
target_include_directories(${LibConfigMngName} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

target_include_directories(${LibConfigMngName} PRIVATE src)
target_include_directories(${LibConfigMngName} PRIVATE inc)
target_include_directories(${LibConfigMngName} PRIVATE api)



set_target_properties( ${LibConfigMngName}  PROPERTIES  VERSION ${CorebrokerCommonLibs_VERSION_MAJOR} SOVERSION ${CorebrokerCommonLibs_VERSION_MINOR} PUBLIC_HEADER "${PUBLIC_HEADER_FILES}" )

install(TARGETS ${LibConfigMngName}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/corebroker/tcan)

    
if( ENABLE_TEST )
    message("Unit test enabled ${CMAKE_CURRENT_SOURCE_DIR}/test/")
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
endif()
message("")
