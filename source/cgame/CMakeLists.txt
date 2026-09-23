file(GLOB CGAME_HEADERS
    "*.h"
	"../gameshared/*.h"
)

file(GLOB CGAME_SOURCES
    "*.cpp"
    "../gameshared/*.c"
)

add_library(cgame MODULE ${CGAME_HEADERS} ${CGAME_SOURCES})
