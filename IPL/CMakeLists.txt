cmake_minimum_required(VERSION 3.10.2)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug, Release, or MinSizeRel." FORCE)
    message(STATUS "CMAKE_BUILD_TYPE not set, defaulting to Release.")
endif ()

project(levinboot-tools LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS TRUE)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_compile_options(-Wall)

find_package(PkgConfig REQUIRED)
pkg_check_modules(USB REQUIRED libusb-1.0)
add_executable(usbtool usbtool.c)
target_include_directories(usbtool PUBLIC ${USB_INCLUDE_DIRS})
target_link_libraries(usbtool PUBLIC ${USB_LINK_LIBRARIES})

add_executable(idbtool idbtool.c)

add_executable(regtool regtool.c regtool_rpn.c)

set(SRC_FILES
  ${PROJECT_SOURCE_DIR}/../compression/
)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/../compression/*.c)
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/../compression/compression.c)
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/../compression/zstdsplit.c)
add_executable(unpacktool unpacktool.c ${SRC_FILES})
target_compile_definitions(unpacktool PRIVATE HAVE_LZ4 HAVE_GZIP HAVE_ZSTD)
