cmake_minimum_required(VERSION 3.13)

project(build-deps NONE)
cmake_host_system_information(
        RESULT build_concurrency_factor
        QUERY NUMBER_OF_LOGICAL_CORES)
set(CMAKE_EXPORT_NO_PACKAGE_REGISTRY ON CACHE "" INTERNAL FORCE)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY ON CACHE "" INTERNAL FORCE)
set(CMAKE_FIND_PACKAGE_NO_PACKAGE_REGISTRY ON CACHE "" INTERNAL FORCE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(autotools_flags
        CFLAGS=-I@OFDB_DEPS_INSTALL_DIR@/include
        CXXFLAGS=-I@OFDB_DEPS_INSTALL_DIR@/include
        LDFLAGS=-L@OFDB_DEPS_INSTALL_DIR@/lib)
set(PKG_CONFIG_PATH PKG_CONFIG_PATH=@OFDB_DEPS_INSTALL_DIR@/lib/pkgconfig)
set(make_command make -j ${build_concurrency_factor})
set(info_dir --infodir=@OFDB_DEPS_INSTALL_DIR@/share/info)

include(ExternalProject)

ExternalProject_Add(zookeeper-client-c
        URL @OFDB_DEPS_SOURCE_DIR@/zookeeper-client-c.tar.gz
        URL_MD5 19c8b10b5afe3a0f8df6b9f268361531
        INSTALL_DIR @OFDB_DEPS_INSTALL_DIR@
        CONFIGURE_COMMAND
            COMMAND
                ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>
                autoreconf -if
            COMMAND
                ${CMAKE_COMMAND} -E chdir <SOURCE_DIR>
                <SOURCE_DIR>/configure
                --prefix=<INSTALL_DIR>
                --without-cppunit
                --disable-shared
        BUILD_COMMAND ${make_command} -C <SOURCE_DIR>
        INSTALL_COMMAND make install -C <SOURCE_DIR>)

