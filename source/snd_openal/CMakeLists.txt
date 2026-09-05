file(GLOB SND_OPENAL_SOURCES
    "*.c"
    "../snd_common/*.c"
    "../gameshared/q_*.c"
)

file(GLOB SND_OPENAL_HEADERS
    "*.h"
	"../gameshared/q_*.h"
	"../client/snd_public.h"
)

add_library(snd_openal SHARED
    ${SND_OPENAL_SOURCES}
    ${SND_OPENAL_HEADERS}
)

target_include_directories(snd_openal PRIVATE "." "../snd_common")

target_link_libraries(snd_openal
    PRIVATE
    m dl
    PkgConfig::OGG
    PkgConfig::VORBISFILE
    PkgConfig::OPENAL
)
