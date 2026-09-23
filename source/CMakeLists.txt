set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(_POSIX_C_SOURCE=200809L)

set(CFLAGS
  -Wall -Wextra
  -Wno-unused
  -Wuninitialized
  -Winit-self
  -Wnull-dereference
  -Wstrict-overflow
  -Winvalid-pch

  # FIXME
  -Wno-null-dereference
  -Wno-clobbered
  -Wno-maybe-uninitialized
  -Wno-array-bounds
  -Wno-stringop-overflow

  -fvisibility=hidden
  -fno-strict-aliasing
)

# cmake expands CFLAGS to a semicolon-delimited string, so we gotta quote the
# whole expression to avoid the command argument parser from splitting up our
# genexp.
add_compile_options("$<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:${CFLAGS}>")

set(QFUSION_CLIENT_NAME warsow)
set(QFUSION_SERVER_NAME wsw_server)

set(PUBLIC_BUILD OFF CACHE BOOL "Public build")
if(PUBLIC_BUILD)
  add_compile_definitions(PUBLIC_BUILD)
endif()

find_package(Git)
if(Git_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    OUTPUT_VARIABLE rev
    RESULT_VARIABLE res
    OUTPUT_STRIP_TRAILING_WHITESPACE # EOL newline
  )
  if(res EQUAL 0)
    add_compile_definitions(SVN_REVSTR=\"${rev}\")
  endif()
endif()

add_subdirectory(angelwrap)
add_subdirectory(cin)
add_subdirectory(ftlib)
add_subdirectory(ref_gl)
add_subdirectory(snd_openal)
add_subdirectory(ui)

add_subdirectory(game)
add_subdirectory(cgame)

add_subdirectory(client)
add_subdirectory(server)

foreach(target ${QFUSION_CLIENT_NAME} ${QFUSION_SERVER_NAME})
  set_target_properties(${target} PROPERTIES OUTPUT_NAME "${target}.${CMAKE_SYSTEM_PROCESSOR}")
endforeach()

foreach(target angelwrap cin ftlib ref_gl snd_openal ui game cgame)
  set_target_properties(${target} PROPERTIES OUTPUT_NAME "${target}_${CMAKE_SYSTEM_PROCESSOR}")
endforeach()

install(
  TARGETS
  ${QFUSION_CLIENT_NAME} ${QFUSION_SERVER_NAME}
  angelwrap cin ftlib ref_gl snd_openal ui game cgame
  RUNTIME DESTINATION .
  LIBRARY DESTINATION libs
)

install(PROGRAMS unix/start_script.sh
  DESTINATION .
  RENAME warsow
)

install(PROGRAMS unix/start_script.sh
  DESTINATION .
  RENAME wsw_server
)
