file(GLOB REF_GL_SOURCES
    "*.c"
    "../gameshared/q_*.c"
    "../qalgo/*.c"
    "../qcommon/bsp.c"
    "../qcommon/patch.c"
)

file(GLOB REF_GL_PLATFORM_SOURCES
    "../sdl/sdl_glw.c"
    "../sdl/sdl_qgl.c"
)

file(GLOB REF_GL_HEADERS
    "*.h"
	"../gameshared/q_*.h"
	"../gameshared/anorms.h"
	"../gameshared/config.h"
	"../qcommon/bsp.h"
	"../qcommon/patch.h"
	"../qcommon/iqm.h"
	"../qcommon/qfiles.h"
	"../qalgo/*.h"
)

add_library(ref_gl SHARED
    ${REF_GL_SOURCES}
    ${REF_GL_PLATFORM_SOURCES}
    ${REF_GL_HEADERS}
)

target_link_libraries(ref_gl
    PRIVATE
    SDL3::SDL3
    PkgConfig::LIBPNG
    PkgConfig::LIBJPEG
)

# qf_set_output_dir(ref_gl libs)
