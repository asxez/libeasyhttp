cmake_minimum_required(VERSION 3.27)

project(libeasyhttp C)

set(CMAKE_TOOLCHAIN_FILE "F:\\WorkSpace\\vcpkg-2023.12.12\\scripts\\buildsystems\\vcpkg.cmake")

if (DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif ()

include_directories(easyhttp)

add_executable(test test.c)

find_package(CURL REQUIRED)
target_link_libraries(test PRIVATE CURL::libcurl)
