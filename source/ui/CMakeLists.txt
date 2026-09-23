file(GLOB UI_HEADERS
    "*.h"
    "kernel/*.h"
    "as/*.h"
    "datasources/*.h"
    "widgets/*.h"
    "decorators/*.h"
    "parsers/*.h"
	"formatters/*.h"
	"../gameshared/q_*.h"
	"../qalgo/*.h"
)

file(GLOB UI_SOURCES
    "*.cpp"
    "kernel/*.cpp"
    "as/*.cpp"
    "datasources/*.cpp"
    "widgets/*.cpp"
    "decorators/*.cpp"
    "parsers/*.cpp"
    "../gameshared/q_*.c"
	"../qalgo/*.c"
)

add_library(ui MODULE ${UI_HEADERS} ${UI_SOURCES})

target_precompile_headers(ui PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/ui_precompiled.h>)

target_include_directories(ui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ui
    PRIVATE
    RocketCore RocketControls
    Angelscript
)
