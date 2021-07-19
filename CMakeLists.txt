cmake_minimum_required(VERSION 3.1)
project(nathanArgon C)

# Standard C11/C17
set(CMAKE_C_STANDARD 11)

add_link_options($<$<CONFIG:RELEASE>:-s>)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native -flto -pthread")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -pthread")

include(ExternalProject)

# Argon2 - Bezpečné hashování
set(ARGON2_SOURCES
        ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/src/argon2.c
        ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/src/core.c
        ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/src/encoding.c
        ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/src/opt.c
        ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/src/thread.c
        ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/src/blake2/blake2b.c
        )
add_library (argon2 ${ARGON2_SOURCES})
target_include_directories (argon2 PRIVATE ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/src ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/src/blake2 ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/include )
target_compile_options(argon2 PRIVATE -O3 -march=native -mavx2 -flto -pthread)

# Parson - JSON knihovna
add_library(parson ${CMAKE_SOURCE_DIR}/deps/parson/parson.c)
target_include_directories(parson PRIVATE ${CMAKE_SOURCE_DIR}/deps/parson)
target_compile_options(parson PRIVATE -O3 -march=native -flto)

# LibmicroHTTPd - Webový HTTP server (Autoconf :/)
set(MICROHTTPD_CONFIGURE_ARGUMENTS
        CFLAGS=-O3
         --enable-shared=no
        --enable-static=yes
        --with-threads=posix
        --disable-https
        --disable-curl
        --disable-doc
        --disable-examples
        --disable-bauth
        --disable-dauth
        --disable-httpupgrade
        --prefix=${CMAKE_CURRENT_BINARY_DIR}/libmicrohttpd/)
ExternalProject_Add(
        libmicrohttpd
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps/libmicrohttpd
        CONFIGURE_COMMAND autoreconf --force --install > /dev/null 2>&1 || autoreconf && ./configure ${MICROHTTPD_CONFIGURE_ARGUMENTS}
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/libmicrohttpd/
        BUILD_COMMAND make -j 4
        BUILD_IN_SOURCE 1
)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/libmicrohttpd/lib)

# Hlavní program
add_executable(nathanArgon main.c)
add_dependencies(nathanArgon libmicrohttpd)

# Linkování knihoven a přidání include souborů
target_link_libraries(nathanArgon argon2 parson microhttpd)
set(NATHANARGON_INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/deps/libmicrohttpd/src/include
        ${CMAKE_SOURCE_DIR}/deps/phc-winner-argon2/include
        ${CMAKE_SOURCE_DIR}/deps/parson)
target_include_directories(nathanArgon PRIVATE ${NATHANARGON_INCLUDE_DIRECTORIES} )
