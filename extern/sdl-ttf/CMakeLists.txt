message(STATUS "Fetching SDL ttf...")

#FetchContent_GetProperties(SDL_ttf)
#if(NOT SDL_ttf_POPULATED)
#    FetchContent_Populate(SDL_ttf)
#endif()

# START ADDITION
set(SDL2TTF_INSTALL OFF)
# END ADDITION

FetchContent_MakeAvailable(SDL2_ttf)