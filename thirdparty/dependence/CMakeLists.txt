# don't export() the contents to registry
set(CMAKE_EXPORT_NO_PACKAGE_REGISTRY ON  CACHE "" INTERNAL FORCE)
# disable system level registry /usr/local/share/cmake/*
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY ON  CACHE "" INTERNAL FORCE)
# disable user package registry ~/.cmake/*
set(CMAKE_FIND_PACKAGE_NO_PACKAGE_REGISTRY ON  CACHE "" INTERNAL FORCE)

set(OFDB_DEPS_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/ofdb-deps-build)
set(OFDB_DEPS_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/ofdb-deps-install)
set(OFDB_DEPS_SOURCE_DIR file://${CMAKE_CURRENT_SOURCE_DIR})

configure_file(${DEP_CMAKE_FILE} ${OFDB_DEPS_BUILD_DIR}/CMakeLists.txt @ONLY)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${OFDB_DEPS_BUILD_DIR})
if(result)
    message(FATAL_ERROR "CMake step for deps-build failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${OFDB_DEPS_BUILD_DIR})
if(result)
    message(FATAL_ERROR "Build step for deps-build failed: ${result}")
endif()
