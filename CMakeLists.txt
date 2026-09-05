cmake_minimum_required(VERSION 3.15)
project(qfusion)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

set(QFUSION_GAME_DIR basewsw)
set(QFUSION_CLIENT_NAME warsow)
set(QFUSION_SERVER_NAME wsw_server)
set(QFUSION_TVSERVER_NAME wswtv_server)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED IMPORTED_TARGET libcurl)
pkg_check_modules(ZLIB REQUIRED IMPORTED_TARGET zlib)
pkg_check_modules(LIBPNG REQUIRED IMPORTED_TARGET libpng)
pkg_check_modules(LIBJPEG REQUIRED IMPORTED_TARGET libjpeg)
pkg_check_modules(OGG REQUIRED IMPORTED_TARGET ogg)
pkg_check_modules(VORBISFILE REQUIRED IMPORTED_TARGET vorbisfile)
pkg_check_modules(THEORADEC REQUIRED IMPORTED_TARGET theoradec)
pkg_check_modules(OPENAL REQUIRED IMPORTED_TARGET openal)

find_package(SDL3 REQUIRED)
find_package(Freetype REQUIRED)

include(FetchContent)

FetchContent_Declare(
  angelscript
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/third-party/angelscript
  EXCLUDE_FROM_ALL
)

FetchContent_Declare(
  libRocket
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/third-party/libRocket
  EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(angelscript libRocket)

# TODO: clean this up
add_definitions(-DAPPLICATION_VERSION_HEADER=\"version.warsow.h\")

add_subdirectory(source)
