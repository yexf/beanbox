set(DEP_CMAKE_FILE ${CMAKE_MODULE_PATH}/deps.cmake)
add_subdirectory(dependence)

add_definitions(-Wextra -Wall -Wsign-compare -Wno-unused-parameter -Woverloaded-virtual -Wnon-virtual-dtor -Wno-missing-field-initializers)
add_definitions(-fno-builtin-memcmp -msse -msse4.2 -fno-omit-frame-pointer -DNDEBUG -momit-leaf-frame-pointer)
add_definitions(-D_GNU_SOURCE -fno-omit-frame-pointer -DNDEBUG -pipe -fPIC -pg)


include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/slash/include
        ${CMAKE_CURRENT_SOURCE_DIR}/pink/include
        ${CMAKE_CURRENT_SOURCE_DIR}/bean/include
)
configure_file(slash/src/build_version.cc.in ${CMAKE_CURRENT_BINARY_DIR}/slash/build_version.cc @ONLY)
configure_file(pink/src/build_version.cc.in ${CMAKE_CURRENT_BINARY_DIR}/pink/build_version.cc @ONLY)
configure_file(bean/src/build_version.cc.in ${CMAKE_CURRENT_BINARY_DIR}/bean/build_version.cc @ONLY)

aux_source_directory(${CMAKE_CURRENT_BINARY_DIR}/slash SLASH_GEN)
aux_source_directory(slash/src SLASH_SRC)
aux_source_directory(${CMAKE_CURRENT_BINARY_DIR}/pink PINK_GEN)
aux_source_directory(pink/src PINK_SRC)
aux_source_directory(${CMAKE_CURRENT_BINARY_DIR}/bean BEAN_GEN)
aux_source_directory(bean/src BEAN_SRC)

add_library(bean SHARED ${BEAN_GEN} ${BEAN_SRC} ${SLASH_GEN} ${SLASH_SRC} ${PINK_GEN} ${PINK_SRC})
target_link_libraries(bean PUBLIC pthread protobuf dl)
