message(STATUS "Fetching SDL ...")

#FetchContent_GetProperties(SDL2)
#if(NOT SDL2_POPULATED)
#    FetchContent_Populate(SDL2)
#endif()

FetchContent_MakeAvailable(SDL2)