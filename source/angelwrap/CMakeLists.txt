file(GLOB ANGELWRAP_HEADERS
    "*.h"
    "addon/*.h"
	"../gameshared/q_*.h"
)

file(GLOB ANGELWRAP_SOURCES
    "*.cpp"
    "*.c"
    "addon/*.cpp"
    "../gameshared/q_*.c"
)

add_library(angelwrap MODULE ${ANGELWRAP_HEADERS} ${ANGELWRAP_SOURCES})
target_link_libraries(angelwrap
    PRIVATE
    Threads::Threads
    Angelscript
)
