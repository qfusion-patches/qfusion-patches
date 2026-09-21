cmake_minimum_required(VERSION 3.15)
project(qfusion)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(SDL3 REQUIRED)
find_package(Freetype REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED IMPORTED_TARGET libcurl)
pkg_check_modules(ZLIB REQUIRED IMPORTED_TARGET zlib)
pkg_check_modules(LIBPNG REQUIRED IMPORTED_TARGET libpng)
pkg_check_modules(LIBJPEG REQUIRED IMPORTED_TARGET libjpeg)
pkg_check_modules(OGG REQUIRED IMPORTED_TARGET ogg)
pkg_check_modules(VORBISFILE REQUIRED IMPORTED_TARGET vorbisfile)
pkg_check_modules(THEORADEC REQUIRED IMPORTED_TARGET theoradec)
pkg_check_modules(OPENAL REQUIRED IMPORTED_TARGET openal)

include(FetchContent)
FetchContent_Declare(
  angelscript
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/third-party/angelscript
  SYSTEM EXCLUDE_FROM_ALL
)
FetchContent_Declare(
  libRocket
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/third-party/libRocket
  SYSTEM EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(angelscript libRocket)

add_subdirectory(source)
