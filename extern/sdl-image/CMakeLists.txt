message(STATUS "Fetching SDL image...")

#FetchContent_GetProperties(SDL2_image)
#if(NOT SDL2_image_POPULATED)
#    FetchContent_Populate(SDL2_image)
#endif()

# START ADDITION
set(SDL2IMAGE_INSTALL OFF)
# END ADDITION

FetchContent_MakeAvailable(SDL2_image)