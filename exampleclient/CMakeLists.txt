cmake_minimum_required(VERSION 3.4)

project(ExampleClient VERSION 1.0 LANGUAGES CXX)

set( ExampleClientName "exampleclient")

include (GNUInstallDirs)

set (SRC_FILES "")
set (HEADER_FILES "")

file(GLOB SRC_FILES
    "./src/*.cpp"
    )

file(GLOB HEADER_FILES
    "./inc/*.h"
    )

file(GLOB PUBLIC_HEADER_FILES
    "api/*.h"
    )



include_directories(${COREBROKER_PRJ_PATH}/common/)
include_directories(${COREBROKER_PRJ_PATH}/common/jsoncpp/)
include_directories(${COREBROKER_PRJ_PATH}/common/tnodenet/inc)
include_directories(${COREBROKER_PRJ_PATH}/common/tmessage/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/)


add_executable( ${ExampleClientName} ${SRC_FILES} )
target_link_libraries(${ExampleClientName} ${LibFileName} ${LibNodeSocketName} ${LibNodeMessageName} )

message("LINK LIBS ${ExampleClientName} ${LibFile} ${LibFileName} ${LibNodeMessage} ${LibNodeSocketName}")



    
#if( ENABLE_TEST )
#    message("Unit test enabled ${CMAKE_CURRENT_SOURCE_DIR}/test/")
#    include(${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
#endif()



