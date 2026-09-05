file(GLOB FTLIB_SOURCES
    "*.c"
    "../gameshared/q_*.c"
)

file(GLOB FTLIB_HEADERS
    "*.h"
	"../gameshared/q_*.h"
)

add_library(ftlib SHARED ${FTLIB_SOURCES} ${FTLIB_HEADERS})
target_link_libraries(ftlib PRIVATE Freetype::Freetype)
