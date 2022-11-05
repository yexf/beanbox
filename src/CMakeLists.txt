set(BUILD_VERSION_CC ${CMAKE_CURRENT_BINARY_DIR}/build_version.cc)
configure_file(build_version.cc.in  ${BUILD_VERSION_CC} @ONLY)

add_definitions(-W -Wextra -Wall -Wsign-compare -Wno-unused-parameter -Woverloaded-virtual -Wnon-virtual-dtor -Wno-missing-field-initializers)
add_definitions(-fno-builtin-memcmp -msse -msse4.2 -fno-omit-frame-pointer -DNDEBUG -momit-leaf-frame-pointer)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
)
aux_source_directory(. ROOT_DIR)
set(SRCS ${BUILD_VERSION_CC}
        ${ROOT_DIR})
add_executable(bbox ${SRCS})
target_link_libraries(bbox bean glog gflags)


add_subdirectory(ofdb)