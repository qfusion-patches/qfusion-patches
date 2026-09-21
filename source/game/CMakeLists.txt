file(GLOB GAME_SOURCES
    "*.cpp"
    "ai/*.cpp"
    "../gameshared/*.c"
)

file(GLOB GAME_HEADERS
    "*.h"
    "ai/*.h"
	"../gameshared/*.h"
)

add_library(game SHARED ${GAME_SOURCES} ${GAME_HEADERS})
target_link_libraries(game PRIVATE Angelscript)
