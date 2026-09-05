file(GLOB SERVER_SOURCES
	"../qcommon/asyncstream.c"
	"../qcommon/autoupdate.c"
    "../qcommon/cm_main.c"
    "../qcommon/cm_q3bsp.c"
    "../qcommon/cm_trace.c"
	"../qcommon/compression.c"
    "../qcommon/bsp.c"
    "../qcommon/patch.c"
    "../qcommon/common.c"
    "../qcommon/files.c"
    "../qcommon/cmd.c"
    "../qcommon/mem.c"
    "../qcommon/net.c"
    "../qcommon/net_chan.c"
    "../qcommon/msg.c"
    "../qcommon/cvar.c"
    "../qcommon/dynvar.c"
    "../qcommon/irc.c"
    "../qcommon/library.c"
    "../qcommon/mlist.c"
    "../qcommon/svnrev.c"
    "../qcommon/snap_demos.c"
    "../qcommon/snap_write.c"
    "../qcommon/ascript.c"
    "../qcommon/anticheat.c"
    "../qcommon/wswcurl.c"
    "../qcommon/cjson.c"
    "../qcommon/threads.c"
    "*.c"
    "../null/cl_null.c"
    "../gameshared/q_*.c"
    "../qalgo/*.c"
)

file(GLOB SERVER_PLATFORM_SOURCES
    "../unix/unix_fs.c"
    "../unix/unix_net.c"
    "../unix/unix_sys.c"
    "../unix/unix_console.c"
    "../unix/unix_time.c"
    "../unix/unix_lib.c"
    "../unix/unix_threads.c"
    "../null/sys_vfs_null.c"
)

file(GLOB SERVER_HEADERS
    "*.h"
	"../gameshared/q_*.h"
	"../gameshared/anorms.h"
	"../gameshared/config.h"
	"../qcommon/*.h"
	"../qalgo/*.h"
	"../angelwrap/qas_public.h"
	"../game/g_public.h"
	"../irc/irc_interface.h"
)

add_executable(${QFUSION_SERVER_NAME}
    ${SERVER_SOURCES}
    ${SERVER_PLATFORM_SOURCES}
    ${SERVER_HEADERS}
)

target_compile_definitions(
    ${QFUSION_SERVER_NAME}
    PRIVATE
    DEDICATED_ONLY
)

target_link_libraries(
    ${QFUSION_SERVER_NAME}
    PRIVATE
    m dl
    game cgame
    Threads::Threads
    PkgConfig::CURL
    PkgConfig::ZLIB
)

add_dependencies(${QFUSION_SERVER_NAME} angelwrap irc)
