message(STATUS "Fetching SDL mixer...")

#FetchContent_GetProperties(SDL_mixer)
#if(NOT SDL_mixer_POPULATED)
#    FetchContent_Populate(SDL_mixer)
#endif()

# START ADDITION
set(SDL2MIXER_INSTALL OFF)
# END ADDITION

FetchContent_MakeAvailable(SDL2_mixer)