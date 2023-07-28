include(FetchContent)

Set(FETCHCONTENT_QUIET OFF)
set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.0
    GIT_PROGRESS TRUE
)
add_subdirectory(sdl)

FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.6.3
    GIT_PROGRESS TRUE
)
add_subdirectory(sdl-image)


FetchContent_Declare(
    SDL2_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-2.6.3
    GIT_PROGRESS TRUE
)
add_subdirectory(sdl-mixer)

FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.20.2
    GIT_PROGRESS TRUE
)
add_subdirectory(sdl-ttf)

FetchContent_Declare(
    SDL2_net
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_net.git
    GIT_TAG release-2.2.0
    GIT_PROGRESS TRUE
)
add_subdirectory(sdl-net)