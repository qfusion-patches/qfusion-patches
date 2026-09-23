file(GLOB GAME_HEADERS
    "*.h"
    "ai/*.h"
	"../gameshared/*.h"
)

file(GLOB GAME_SOURCES
    "*.cpp"
    "ai/*.cpp"
    "../gameshared/*.c"
)

add_library(game MODULE ${GAME_HEADERS} ${GAME_SOURCES})
target_link_libraries(game PRIVATE Angelscript)
