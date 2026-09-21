set(QFUSION_GAME_DIR basewsw)
set(QFUSION_CLIENT_NAME warsow)
set(QFUSION_SERVER_NAME wsw_server)
set(QFUSION_TVSERVER_NAME wswtv_server)
add_compile_definitions(APPLICATION_VERSION_HEADER=\"version.warsow.h\")
add_compile_definitions($<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:PUBLIC_BUILD>)

add_compile_definitions(_POSIX_C_SOURCE=200809L)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CFLAGS
  -Wall -Wextra
  -Wno-unused
  -Wuninitialized
  -Winit-self
  -Wnull-dereference
  -Wstrict-overflow
  -Winvalid-pch

  -fvisibility=hidden
  -fno-strict-aliasing
)

# cmake expands CFLAGS to a semicolon-delimited string, so we gotta quote the
# whole expression to avoid the command argument parser from splitting up our
# genexp.
add_compile_options("$<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:${CFLAGS}>")

add_subdirectory(angelwrap)

add_subdirectory(cin)
add_subdirectory(ftlib)
add_subdirectory(ref_gl)
add_subdirectory(snd_openal)

add_subdirectory(game)
add_subdirectory(cgame)

add_subdirectory(client)
add_subdirectory(server)

add_subdirectory(ui)
