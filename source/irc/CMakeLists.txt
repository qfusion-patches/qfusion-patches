file(GLOB IRC_SOURCES "*.c")
file(GLOB IRC_HEADERS "*.h")

add_library(irc SHARED ${IRC_SOURCES} ${IRC_HEADERS})
target_link_libraries(irc PRIVATE m)
