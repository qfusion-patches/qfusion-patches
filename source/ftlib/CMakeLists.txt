file(GLOB FTLIB_HEADERS
    "*.h"
	"../gameshared/q_*.h"
)

file(GLOB FTLIB_SOURCES
    "*.c"
    "../gameshared/q_*.c"
)

add_library(ftlib MODULE ${FTLIB_HEADERS} ${FTLIB_SOURCES})
target_link_libraries(ftlib PRIVATE Freetype::Freetype)
