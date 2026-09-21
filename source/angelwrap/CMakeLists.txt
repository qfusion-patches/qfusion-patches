file(GLOB ANGELWRAP_SOURCES
    "*.cpp"
    "*.c"
    "addon/*.cpp"
    "../gameshared/q_*.c"
)

file(GLOB ANGELWRAP_HEADERS
    "*.h"
    "addon/*.h"
	"../gameshared/q_*.h"
)

add_library(angelwrap SHARED ${ANGELWRAP_SOURCES} ${ANGELWRAP_HEADERS})

target_link_libraries(angelwrap
    PRIVATE
    Threads::Threads
    Angelscript
)
