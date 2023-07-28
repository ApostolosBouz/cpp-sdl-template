message(STATUS "Fetching SDL net...")

#FetchContent_GetProperties(SDL_net)
#if(NOT SDL_net_POPULATED)
#    FetchContent_Populate(SDL_net)
#endif()

# START ADDITION
set(SDL2NET_INSTALL OFF)
# END ADDITION

FetchContent_MakeAvailable(SDL2_net)