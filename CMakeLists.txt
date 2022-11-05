cmake_minimum_required(VERSION 3.13)
project(ofdb)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("==>>Debug Model")
    SET(CMAKE_VERBOSE_MAKEFILE ON)
    add_definitions("-Wall -Wno-sign-compare -g -Wno-strict-aliasing -Wno-comment")
else()
    message("==>>Release Model")
    add_definitions("-Wall -Wno-sign-compare -g -O2 -Wno-strict-aliasing -Wno-comment")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#find_package(PythonLibs REQUIRED)
#-------------------------------------------------------------------------------
# Set include paths and library paths.
#-------------------------------------------------------------------------------
message("==>>${PROJECT_SOURCE_DIR}/include")
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
)
link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

string(TIMESTAMP GIT_DATE_TIME "%Y/%m/%d %H:%M:%S" UTC)
find_package(Git)
if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    if(WIN32)
        execute_process(COMMAND $ENV{COMSPEC} /C ${GIT_EXECUTABLE} -C ${CMAKE_CURRENT_SOURCE_DIR} rev-parse HEAD OUTPUT_VARIABLE GIT_SHA)
    else()
        execute_process(COMMAND ${GIT_EXECUTABLE} -C ${CMAKE_CURRENT_SOURCE_DIR} rev-parse HEAD OUTPUT_VARIABLE GIT_SHA)
    endif()
else()
    set(GIT_SHA 0)
endif()

string(REGEX REPLACE "[^0-9a-f]+" "" GIT_SHA "${GIT_SHA}")

add_subdirectory(thirdparty)
add_subdirectory(src)
