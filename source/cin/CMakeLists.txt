file(GLOB CIN_SOURCES
    "*.c"
    "../gameshared/q_*.c"
)

file(GLOB CIN_HEADERS
    "*.h"
	"../gameshared/q_*.h"
)

add_library(cin SHARED ${CIN_SOURCES} ${CIN_HEADERS})
target_link_libraries(cin
    PRIVATE
    m
    PkgConfig::OGG
    PkgConfig::VORBISFILE
    PkgConfig::THEORADEC
)
