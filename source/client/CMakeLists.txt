file(GLOB CLIENT_SOURCES
    "../qcommon/*.c"
    "../server/*.c"
    "*.c"
    "../gameshared/q_*.c"
    "../qalgo/*.c"
)

file(GLOB CLIENT_PLATFORM_SOURCES
    "../unix/unix_fs.c"
    "../unix/unix_net.c"
    "../unix/unix_sys.c"
    "../unix/unix_time.c"
    "../unix/unix_lib.c"
    "../unix/unix_threads.c"
    "../null/sys_vfs_null.c"

    "../sdl/sdl_input.c"
    "../sdl/sdl_vid.c"
    "../sdl/sdl_sys.c"
    "../sdl/sdl_clipboard.c"
    "../sdl/sdl_client.c"
)

file(GLOB CLIENT_HEADERS
    "*.h"
	"../gameshared/q_*.h"
	"../gameshared/anorms.h"
	"../gameshared/config.h"
	"../qcommon/*.h"
	"../qalgo/*.h"
	"../angelwrap/qas_public.h"
	"../ref_gl/r_public.h"
	"../cgame/cg_public.h"
	"../cgame/ref.h"
	"../game/g_public.h"
	"../ui/ui_public.h"
	"../ftlib/ftlib_public.h"
	"../irc/irc_interface.h"
	"../cin/cin_public.h"
)

add_executable(${QFUSION_CLIENT_NAME}
    ${CLIENT_SOURCES}
    ${CLIENT_PLATFORM_SOURCES}
    ${CLIENT_HEADERS}
)

target_link_libraries(${QFUSION_CLIENT_NAME}
    PRIVATE
    # game cgame
    m dl rt
    Threads::Threads
    SDL3::SDL3
    PkgConfig::CURL
    PkgConfig::ZLIB
)

add_dependencies(${QFUSION_CLIENT_NAME} game cgame ui cin ftlib ref_gl irc snd_openal)
